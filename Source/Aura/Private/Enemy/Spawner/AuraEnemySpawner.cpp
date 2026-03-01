#include "Enemy/Spawner/AuraEnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraEnemy.h"

AAuraEnemySpawner::AAuraEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");
	SpawnArea->SetupAttachment(GetRootComponent());

	// Configuración de GAS para el Spawner
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraEnemySpawner::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// Inicializamos la info del actor para GAS
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	if (EnemyClass)
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AAuraEnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
}

void AAuraEnemySpawner::SpawnEnemy()
{
	if (!EnemyClass) return;
	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent());
	GetWorld()->SpawnActor<AAuraEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
}

void AAuraEnemySpawner::Die()
{
	// Detenemos el temporizador para que no aparezcan más enemigos
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	
	// Efectos visuales de destrucción (opcional) o simplemente destruir el actor
	Destroy();
}