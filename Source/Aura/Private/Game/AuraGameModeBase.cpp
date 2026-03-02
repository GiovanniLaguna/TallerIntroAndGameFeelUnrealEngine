// Copyrigh Quetz-Lab 2026 


#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Enemy/Spawner/AuraEnemySpawner.h" 
#include "AuraEnemy.h"    

void AAuraGameModeBase::CheckWinCondition()
{
	// 1. Obtener todos los Spawners
	TArray<AActor*> AllSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAuraEnemySpawner::StaticClass(), AllSpawners);

	int32 SpawnersVivos = 0;
	for (AActor* Spawner : AllSpawners)
	{
		// Solo lo contamos si NO se está destruyendo en este momento
		if (IsValid(Spawner) && !Spawner->IsActorBeingDestroyed())
		{
			SpawnersVivos++;
		}
	}

	// 2. Obtener todos los Enemigos
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAuraEnemy::StaticClass(), AllEnemies);

	int32 EnemigosVivos = 0;
	for (AActor* Enemy : AllEnemies)
	{
		// Solo lo contamos si es válido, no se está destruyendo, y su LifeSpan es 0 (No es un cadáver)
		if (IsValid(Enemy) && !Enemy->IsActorBeingDestroyed() && Enemy->GetLifeSpan() == 0.f)
		{
			EnemigosVivos++;
		}
	}

	// (Opcional) El texto azul para que veas la cuenta real sin cadáveres
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Spawners Vivos: %d | Enemigos Vivos: %d"), SpawnersVivos, EnemigosVivos));
	}

	// 3. Si ya no hay vivos reales... ¡Victoria!
	if (SpawnersVivos == 0 && EnemigosVivos == 0)
	{
		ShowVictoryScreen();
	}
}
