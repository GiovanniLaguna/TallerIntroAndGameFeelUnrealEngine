// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "AuroPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuroPlayer : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuroPlayer();	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Die() override;
	
	// Evento que llamará al Widget de Game Over desde el Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Aura|UI")
	void ShowGameOverScreen();
private: 
	void InitAbilityActorInfo();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
	
	// Esta función escuchará cada vez que la vida cambie
	void OnHealthChanged(const struct FOnAttributeChangeData& Data);
};
