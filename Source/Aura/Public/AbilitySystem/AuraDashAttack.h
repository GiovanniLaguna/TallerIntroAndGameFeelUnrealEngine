#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h" // Heredamos de la clase base de Unreal
#include "AuraDashAttack.generated.h"

UCLASS()
class AURA_API UAuraDashAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAuraDashAttack();

	// Función principal que dispara la habilidad
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashImpulse = 2500.f;

	// El efecto de daño que ya creamos (GE_MeleeDamage)
	UPROPERTY(EditAnywhere, Category = "Dash")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Radio de la esfera que busca enemigos mientras haces el dash
	UPROPERTY(EditAnywhere, Category = "Dash")
	float CollisionRadius = 100.f;

	void CheckDashCollision();

private:
	FTimerHandle DashTimerHandle;
};