// Copyrigh Quetz-Lab 2026 

#include "UI/WidgetController/OverlayWidgetController.h"
#include  "UI/WidgetController/AuraWidgetController.h"
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
	Super::BindCallbacksToDependencies();
	
	if (AbilitySystemComponent)
	{
		// 1. Conectamos nuestro "oído" al Ability System Component del jugador
		AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &UOverlayWidgetController::OnGameplayEffectApplied);
	}
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

void UOverlayWidgetController::OnGameplayEffectApplied(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// 1. Extraemos las etiquetas descriptivas (Asset Tags)
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	// 2. Extraemos las etiquetas que se le pegan al jugador (Granted Tags)
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	// 3. Juntamos ambas listas en una sola para no perder ninguna
	FGameplayTagContainer AllTags;
	AllTags.AppendTags(AssetTags);
	AllTags.AppendTags(GrantedTags);

	// 4. Ahora sí, revisamos la lista completa
	for (const FGameplayTag& Tag : AllTags)
	{
		// Buscamos si la etiqueta contiene la palabra "Cooldown"
		if (Tag.ToString().Contains("Cooldown"))
		{
			float Duration = SpecApplied.GetDuration();
			
			// Le avisamos al Blueprint
			OnCooldownChanged.Broadcast(Tag, Duration, Duration);
		}
	}
}