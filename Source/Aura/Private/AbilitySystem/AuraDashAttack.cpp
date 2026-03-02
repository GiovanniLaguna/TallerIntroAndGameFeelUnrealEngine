#include "AbilitySystem/AuraDashAttack.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAuraDashAttack::UAuraDashAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAuraDashAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// --- ¡ESTA ES LA LÍNEA MÁGICA QUE FALTABA! ---
	// Esto le dice a GAS: "Aplica el Cooldown y el Costo de maná/energía ahora mismo"
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// Si falla el commit (ej. si ya estaba en cooldown), cancelamos la habilidad
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character) return;

	// 1. Impulso físico: Lanzamos al personaje hacia adelante
	FVector DashDirection = Character->GetActorForwardVector();
	Character->LaunchCharacter(DashDirection * DashImpulse, true, true);

	// 2. Iniciamos el rastro de daño: Revisamos colisiones cada 0.05s
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAuraDashAttack::CheckDashCollision, 0.05f, true);

	// 3. Duración del Dash: Después de 0.4s detenemos todo
	FTimerHandle StopHandle;
	GetWorld()->GetTimerManager().SetTimer(StopHandle, [this]() {
		GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}, 0.4f, false);
}

void UAuraDashAttack::CheckDashCollision()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	TArray<AActor*> Ignored;
	Ignored.Add(Avatar);

	TArray<FHitResult> HitResults;
	
	// Dibujamos una esfera invisible (o visible para debug) que daña al contacto
	UKismetSystemLibrary::SphereTraceMulti(
		Avatar,
		Avatar->GetActorLocation(),
		Avatar->GetActorLocation(),
		CollisionRadius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		Ignored,
		EDrawDebugTrace::ForDuration, // Cambiar a None cuando termines de probar
		HitResults,
		true
	);

	for (const FHitResult& Hit : HitResults)
	{
		if (UAbilitySystemComponent* TargetASC = Hit.GetActor()->FindComponentByClass<UAbilitySystemComponent>())
		{
			// Aplicamos el daño (reutilizando tu lógica de ataque base)
			FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, 1.f, Context);
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
		}
	}
}