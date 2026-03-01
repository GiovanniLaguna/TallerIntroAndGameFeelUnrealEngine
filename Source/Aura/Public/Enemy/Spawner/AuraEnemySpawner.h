#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h" // Necesario para GAS
#include "Interaction/EnemyInterface.h"
#include "AuraEnemySpawner.generated.h"

class UBoxComponent;
class AAuraEnemy;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API AAuraEnemySpawner : public AActor, public IAbilitySystemInterface, public IEnemyInterface
{
	GENERATED_BODY()
	
public:	
	AAuraEnemySpawner();
	// Funciones de la interfaz (puedes dejarlas vacías por ahora)
	virtual void HighlightActor() override {}
	virtual void UnHighlightActor() override {}
	// Implementación de la interfaz GAS
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Función para detener el spawner cuando su vida llegue a cero
	void Die();

protected:
	virtual void BeginPlay() override;
	void SpawnEnemy();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> SpawnArea;

	// Componentes de GAS para que el Spawner tenga vida y reciba daño
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval = 3.f;

private:
	FTimerHandle SpawnTimerHandle;
};