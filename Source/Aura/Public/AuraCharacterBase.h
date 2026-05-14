// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	UFUNCTION(BlueprintCallable)
	virtual void Die();

	// Función pública para activar el efecto de daño desde fuera (ej. desde el AttributeSet)
	void PlayDamageEffect();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, Category = "Aura|Loot")
	TSubclassOf<class AAuraEffectActor> LootItemClass;
	
	UPROPERTY(EditAnywhere, Category = "Aura|Loot")
	float LootDropChance = 20.f;

	/** Efectos Visuales de Daño **/

	// Instancia dinámica que crearemos en tiempo de ejecución
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

	// Nombre del parámetro en el Master Material
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName DamageParameterName = FName("IsDamaged");

	// Función para apagar el rojo
	void ResetDamageEffect();

	FTimerHandle DamageTimerHandle;
};