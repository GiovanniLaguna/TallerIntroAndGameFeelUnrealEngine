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
private: 
	void InitAbilityActorInfo();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
};
