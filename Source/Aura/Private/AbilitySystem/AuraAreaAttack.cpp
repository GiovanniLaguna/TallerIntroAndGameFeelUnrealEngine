// Copyrigh Quetz-Lab 2026 


#include "AbilitySystem/AuraAreaAttack.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraAreaAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	TArray<AActor*> Ignored;
	Ignored.Add(Avatar);

	TArray<FHitResult> HitResults;
	
	// Realizamos un barrido circular (SphereTrace) para encontrar enemigos
	UKismetSystemLibrary::SphereTraceMulti(
		Avatar,
		Avatar->GetActorLocation(),
		Avatar->GetActorLocation(),
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		Ignored,
		EDrawDebugTrace::ForDuration, // Para que veas el área roja en el editor
		HitResults,
		true
	);

	for (const FHitResult& Hit : HitResults)
	{
		if (UAbilitySystemComponent* TargetASC = Hit.GetActor()->FindComponentByClass<UAbilitySystemComponent>())
		{
			// Aplicamos el GameplayEffect a cada enemigo alcanzado
			FGameplayEffectContextHandle Context = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(DamageEffectClass, 1.f, Context);
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}