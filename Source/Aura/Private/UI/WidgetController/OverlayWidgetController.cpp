// Copyrigh Quetz-Lab 2026 

#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h" // <-- ESTE INCLUDE ES VITAL PARA QUE FUNCIONE

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealtChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealtChanged.Broadcast(AuraAttributeSet->GetMaxHealth()); // <-- Cámbialo a OnMaxHealtChanged
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Usamos FOnAttributeChangeData (Este es el nombre real de la estructura)
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealtChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealtChanged.Broadcast(Data.NewValue);
			}
		);
}