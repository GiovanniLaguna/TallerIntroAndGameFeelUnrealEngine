// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
AAuraEnemy();
/** Enemy Interface*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	virtual void Die() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void PlayDeathShake();

	// Función que llamará la Inteligencia Artificial
	UFUNCTION(BlueprintCallable, Category = "AI")
	void FireRangedAttack();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRadius = 150.f; // Ahora sí aparecerá en el Blueprint del Hechicero y el Esbirro
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void FireMeleeAttack();

protected:

	virtual void BeginPlay() override;
	// Arreglo para darle habilidades iniciales al enemigo desde el Blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
	
};
