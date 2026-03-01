// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);	
	}
	
	
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
	// ¡Aquí conectamos tu nuevo AttackAction!
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AAuraPlayerController::Attack);

	// Registramos la acción del Dash
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AAuraPlayerController::Dash);
	
	// Bindeamos la nueva acción
	EnhancedInputComponent->BindAction(AreaAttackAction, ETriggerEvent::Started, this, &AAuraPlayerController::AreaAttack);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();
	
	/**
	 * Line Trace from cursor. There are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *		-Do Nothing
	 * B. LastActor is null && ThisActor is valid
	 *		-Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		-UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		-UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same acotr
	 *		-Do Nothing
	 */
	
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			//Case B
			
			ThisActor->HighlightActor();
		}
		else
		{
			{
				//Case A - Both are null, do nothing
			}
		}
	}
	else //LastActor is Valid
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHighlightActor();
		}
		else //both actors are valid
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else //Case E - Do nothing
			{
				
			}
		}
	}
}

void AAuraPlayerController::Attack()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Obtenemos el sistema de habilidades del personaje
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ControlledPawn))
		{
			// Le decimos: "Activa la habilidad que tenga la etiqueta Input.Attack"
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Input.Attack")));
			
			ASC->TryActivateAbilitiesByTag(TagContainer);
		}
	}
}

void AAuraPlayerController::Dash()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// Obtenemos el sistema de habilidades del personaje usando Globals
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ControlledPawn))
		{
			// Le decimos: "Activa la habilidad que tenga la etiqueta Input.Dash"
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Input.Dash")));
          
			ASC->TryActivateAbilitiesByTag(TagContainer);
		}
	}
}

void AAuraPlayerController::AreaAttack()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ControlledPawn))
		{
			// Usamos la etiqueta para activar la habilidad
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Input.AreaAttack")));
			ASC->TryActivateAbilitiesByTag(TagContainer);
		}
	}
}