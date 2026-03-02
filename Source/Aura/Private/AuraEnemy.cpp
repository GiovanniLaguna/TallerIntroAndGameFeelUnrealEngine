// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEnemy.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include  "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	// Le decimos que siempre tome su cerebro, ya sea puesto a mano o por Spawner
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Asegúrate de tener el include arriba: #include "AbilitySystemComponent.h"

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent()) // O tu variable del ASC
	{
		// Recorremos la lista y le damos cada habilidad
		for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

void AAuraEnemy::FireRangedAttack()
{
	// Buscamos el sistema de habilidades del enemigo
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this))
	{
		// Le decimos que active la habilidad con la etiqueta del disparo
		// (Asegúrate de que este tag sea exactamente el mismo que pusiste en tu GA_HechiceroDisparo)
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.RangedDamage")));
        
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void AAuraEnemy::FireMeleeAttack()
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this))
	{
		FGameplayTagContainer TagContainer;
		// Esta etiqueta es exclusiva para los golpes físicos
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.MeleeAttack")));
        
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}