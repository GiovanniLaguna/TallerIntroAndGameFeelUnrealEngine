// Copyrigh Quetz-Lab 2026 

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public: 
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly ,Replicated = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly ,Replicated = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly ,Replicated = OnRep_Mana, Category = "Magic Attributes")
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly ,Replicated = OnRep_MaxMana, Category = "Magic Attributes")
	FGameplayAttributeData MaxMana;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
};


