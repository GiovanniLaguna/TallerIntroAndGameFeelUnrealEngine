// Copyrigh Quetz-Lab 2026 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "GameplayEffect.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealtChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealtChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChangedSignature, FGameplayTag, CooldownTag, float, TimeRemaining, float, Duration);


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnHealtChangedSignature OnHealtChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnMaxHealtChangedSignature OnMaxHealtChanged;
	
	virtual void BindCallbacksToDependencies() override;
	
	// Este es el evento rojo que aparecerá en Blueprint
    	UPROPERTY(BlueprintAssignable, Category="Aura|UI")
    	FOnCooldownChangedSignature OnCooldownChanged;
    	
    	protected:
        	//Esta función se disparará automáticamente cuando te apliquen cualquier efecto
        	void OnGameplayEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};
