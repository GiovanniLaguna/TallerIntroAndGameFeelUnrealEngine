#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraMeleeAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystemGlobals.h"

UAuraMeleeAttack::UAuraMeleeAttack()
{
	// Las habilidades de ataque suelen instanciarse por actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UAuraMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Verificamos si podemos ejecutar la habilidad (costos, cooldowns, etc)
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	// 1. Creamos el Task para reproducir la animación
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, 
		NAME_None, 
		AttackMontage, 
		1.f, 
		NAME_None, 
		true
	);

	// 2. Nos suscribimos a los eventos del Montage para terminar la habilidad
	PlayMontageTask->OnCompleted.AddDynamic(this, &UAuraMeleeAttack::OnMontageCompleted);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UAuraMeleeAttack::OnMontageCompleted);
	PlayMontageTask->OnCancelled.AddDynamic(this, &UAuraMeleeAttack::OnMontageCompleted);
	PlayMontageTask->OnBlendOut.AddDynamic(this, &UAuraMeleeAttack::OnMontageCompleted);

	// ... (código previo del Montage) ...

	// Escuchamos el evento que mandará la animación (usaremos un GameplayTag)
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, 
		FGameplayTag::RequestGameplayTag(FName("Event.MeleeAttack.Hit")), 
		nullptr, 
		false, 
		false
	);
	
	WaitEventTask->EventReceived.AddDynamic(this, &UAuraMeleeAttack::OnWeaponHit);
	WaitEventTask->ReadyForActivation();

	// Iniciamos el Task del Montage (esto ya lo tenías)
	PlayMontageTask->ReadyForActivation();
}

void UAuraMeleeAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UAuraMeleeAttack::OnWeaponHit(FGameplayEventData Payload)
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!AvatarActor) return;

	FVector StartLocation = AvatarActor->GetActorLocation();
	FVector EndLocation = StartLocation + (AvatarActor->GetActorForwardVector() * 75.f);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor); 
	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::SphereTraceMulti(
		AvatarActor,
		StartLocation,
		EndLocation,
		120.f, // Radio del ataque
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, // Lo apagamos para producción
		HitResults,
		true
	);

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		
		// Verificamos si es un enemigo válido
		if (HitActor && HitActor->Implements<UEnemyInterface>())
		{
			// Obtenemos el Ability System Component del enemigo
			UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(HitActor);
			
			// Si tiene ASC y tenemos un efecto de daño configurado, lo aplicamos
			if (TargetASC && DamageEffectClass)
			{
				FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
				ContextHandle.AddSourceObject(this);
				
				FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);
				
				GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}
		}
	}
}