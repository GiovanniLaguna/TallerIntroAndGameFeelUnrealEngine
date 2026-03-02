// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Actor/AuraEffectActor.h"
#include "Game/AuraGameModeBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::Die()
{
	// 1. Soltar el ítem (Loot) con Probabilidad
	if (LootItemClass)
	{
		// Tiramos un dado del 1 al 100. Solo suelta el ítem si cae dentro del porcentaje (Ej. 20%)
		if (FMath::RandRange(1, 100) <= LootDropChance) 
		{
			GetWorld()->SpawnActor<AAuraEffectActor>(LootItemClass, GetActorLocation(), GetActorRotation());
		}
	}

	// 2. Desactivar el controlador (SOLO si es un enemigo)
	if (AController* BaseController = GetController())
	{
		// Preguntamos si este controlador NO es de un jugador
		if (!BaseController->IsPlayerController())
		{
			BaseController->UnPossess();
		}
	}
	// 3. Configurar Ragdoll en la malla (Mesh)
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// 4. Desactivar la colisión de la cápsula (para que el jugador no choque con el cadáver)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 5. Opcional: Programar la destrucción después de unos segundos para no saturar la memoria
	SetLifeSpan(5.f); 
	// Le avisamos al GameMode que revise si ya ganamos
	if (AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->CheckWinCondition();
	}
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

