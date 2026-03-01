#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraMeleeAttack.generated.h"

UCLASS()
class AURA_API UAuraMeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UAuraMeleeAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// El Montage que contendrá las animaciones de ataque
	UPROPERTY(EditDefaultsOnly, Category = "Aura|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	// Función que procesará la colisión y los enemigos golpeados
	UFUNCTION()
	void OnWeaponHit(FGameplayEventData Payload);
	
	// Función para terminar la habilidad cuando la animación acabe
	UFUNCTION()
	void OnMontageCompleted();
	
	// Aquí asignaremos el GE_MeleeDamage en el editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|Damage")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;
};