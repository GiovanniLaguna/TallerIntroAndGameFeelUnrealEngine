#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraAreaAttack.generated.h"

UCLASS()
class AURA_API UAuraAreaAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Area Attack")
	float Radius = 500.f; // El radio que pide tu diseño

	UPROPERTY(EditAnywhere, Category = "Area Attack")
	TSubclassOf<UGameplayEffect> DamageEffectClass; // Tu efecto de "daño masivo"
};