// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroPlayer.h"

#include "AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/HUD/AuraHUD.h"

AAuroPlayer::AAuroPlayer()
{

	//Rotacion del personaje
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;


	//Rotacion de eje cancelada
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AAuroPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//Init Ability Actor Info for the server
	InitAbilityActorInfo();
}

void AAuroPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//Init Ability Actor Info for the client
	InitAbilityActorInfo();
}

void AAuroPlayer::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
 	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
 	{
 		if (AAuraHUD* AuraHUD =	Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
 		{
 			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
 		}
 	}
	
	// Otorgar habilidades iniciales (Solo en el servidor para evitar desincronización)
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
		{
			// Le damos la habilidad nivel 1
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}
