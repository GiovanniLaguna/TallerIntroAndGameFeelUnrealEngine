#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraHazardActor.generated.h"

class UBoxComponent;
class UGameplayEffect;

UCLASS()
class AURA_API AAuraHazardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraHazardActor();
	// Evento que el Blueprint escuchará para encender o apagar el fuego
	UFUNCTION(BlueprintImplementableEvent, Category = "Aura|Hazard")
	void OnHazardStateChanged(bool bIsNowActive);
	
protected:
	virtual void BeginPlay() override;

	// La zona que detectará a los personajes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> DamageZone;

	// El efecto de daño de GAS (Ej. GE_HazardDamage)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura|Hazard")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// Tiempos de la trampa
	UPROPERTY(EditAnywhere, Category = "Aura|Hazard")
	float ActiveTime = 3.f; // Cuánto tiempo dura encendida

	UPROPERTY(EditAnywhere, Category = "Aura|Hazard")
	float InactiveTime = 2.f; // Cuánto tiempo dura apagada

	// Estado actual de la trampa (Encendida/Apagada)
	UPROPERTY(BlueprintReadOnly, Category = "Aura|Hazard")
	bool bIsActive;

	// Función que cambia el estado
	void ToggleHazard();
	FTimerHandle HazardTimer;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};