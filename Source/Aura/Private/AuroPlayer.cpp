// Fill out your copyright notice in the Description page of Project Settings.


#include "AuroPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"

AAuroPlayer::AAuroPlayer()
{

//Rotacion del personaje
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,400.f,0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	

//Rotacion de eje cancelada
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
