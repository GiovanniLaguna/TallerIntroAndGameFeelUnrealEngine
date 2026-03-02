#include "Enemy/Spawner/AuraEnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraEnemy.h"
#include "Game/AuraGameModeBase.h"

AAuraEnemySpawner::AAuraEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");
	SpawnArea->SetupAttachment(GetRootComponent());
	SpawnArea->SetCollisionResponseToAllChannels(ECR_Ignore);

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
    
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// PRUEBA 1: ¿Sabe qué enemigo generar?
	if (EnemyClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Spawner: Clase detectada, iniciando temporizador..."));
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AAuraEnemySpawner::SpawnEnemy, SpawnInterval, true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Spawner ERROR: EnemyClass está vacío (None)."));
	}
}

void AAuraEnemySpawner::SpawnEnemy()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Spawner: Intentando hacer aparecer enemigo..."));
    
	if (!EnemyClass) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent());
    
	AAuraEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AAuraEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	// PRUEBA 2: ¿Logró crearlo físicamente?
	if (SpawnedEnemy)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Spawner: ¡Éxito! Enemigo creado."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Spawner ERROR: El motor bloqueó la aparición."));
	}
}

void AAuraEnemySpawner::Die()
{
	// Detenemos el temporizador para que no aparezcan más enemigos
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	
	if (AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->CheckWinCondition();
	}
	
	// Efectos visuales de destrucción (opcional) o simplemente destruir el actor
	Destroy();
}