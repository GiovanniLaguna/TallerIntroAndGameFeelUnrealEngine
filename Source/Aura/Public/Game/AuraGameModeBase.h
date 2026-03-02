// Copyrigh Quetz-Lab 2026 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Función que revisará si ya no hay enemigos ni spawners
	void CheckWinCondition();

	// Evento para que el Blueprint muestre la pantalla de victoria
	UFUNCTION(BlueprintImplementableEvent, Category = "Aura|UI")
	void ShowVictoryScreen();
	
};
