// Fill out your copyright notice in the Description page of Project Settings.

#include "AuraCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Actor/AuraEffectActor.h"
#include "Game/AuraGameModeBase.h"
#include "TimerManager.h" // Necesario para el FTimerHandle

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Solo para testear si funciona al arrancar el juego
	GetWorldTimerManager().SetTimerForNextTick(this, &AAuraCharacterBase::PlayDamageEffect);
	
	// Creamos la instancia dinámica basada en el material actual del Mesh
	if (GetMesh())
	{
		// Forzamos la creación y la asignación manual
		UMaterialInterface* CurrentMat = GetMesh()->GetMaterial(0);
		if (CurrentMat)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(CurrentMat, this);
			GetMesh()->SetMaterial(0, DynamicMaterial); // Re-asignación explícita
		}
		// Si quieres que el arma también parpadee en rojo al recibir daño:
		if (Weapon && DynamicMaterial)
		{
			Weapon->SetMaterial(0, DynamicMaterial);
		}
	}
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("IsDamaged"), 1.0f);
		UE_LOG(LogTemp, Warning, TEXT("TEST: Forzando material rojo en %s"), *GetName());
	}
}

void AAuraCharacterBase::PlayDamageEffect()
{
	if (DynamicMaterial)
	{
		// Encendemos el parámetro del Master Material (IsDamaged = 1.0)
		DynamicMaterial->SetScalarParameterValue(DamageParameterName, 1.0f);

		// Programamos el reset para que sea un parpadeo rápido (0.2 segundos)
		GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AAuraCharacterBase::ResetDamageEffect, 0.2f);
	}
}

void AAuraCharacterBase::ResetDamageEffect()
{
	if (DynamicMaterial)
	{
		// Apagamos el tinte rojo (IsDamaged = 0.0)
		DynamicMaterial->SetScalarParameterValue(DamageParameterName, 0.0f);
	}
}

void AAuraCharacterBase::Die()
{
	// 1. Soltar el ítem (Loot)
	if (LootItemClass)
	{
		if (FMath::RandRange(1, 100) <= LootDropChance) 
		{
			GetWorld()->SpawnActor<AAuraEffectActor>(LootItemClass, GetActorLocation(), GetActorRotation());
		}
	}

	// 2. Desactivar el controlador
	if (AController* BaseController = GetController())
	{
		if (!BaseController->IsPlayerController())
		{
			BaseController->UnPossess();
		}
	}

	// 3. Configurar Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// 4. Desactivar cápsula
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}