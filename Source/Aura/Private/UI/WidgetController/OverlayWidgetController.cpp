// Copyrigh Quetz-Lab 2026 


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealtChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnHealtChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
}
