#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraWeapon.generated.h"

class USphereComponent;
class AAuraCharacterBase;
class UProjectileMovementComponent;
class UGameplayEffect;

UCLASS()
class AURA_API AAuraWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraWeapon();

	virtual void Tick(float DeltaTime) override;

	// Call to attach the weapon to a character
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PickUp(AAuraCharacterBase* NewOwner);

	// Call to throw the weapon in a direction
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ThrowWeapon(FVector Direction);

	// The effect to apply when hitting an enemy
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ThrowDamage = 20.f;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	// Area to overlap and pickup or damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComponent;
	
	// Handles the throw movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

private:
	bool bIsEquipped = false;
	bool bIsThrown = false;
	
	UPROPERTY()
	TObjectPtr<AAuraCharacterBase> CurrentOwnerCharacter;
};
