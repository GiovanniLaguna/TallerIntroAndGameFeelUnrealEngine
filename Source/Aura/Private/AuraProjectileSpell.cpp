#include "AuraProjectileSpell.h"
#include "AuraProjectile.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacter* AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!AvatarCharacter || !ProjectileClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 1. Calcular de dónde sale (un poco adelante del Hechicero para que no choque con él mismo)
	FVector SpawnLocation = AvatarCharacter->GetActorLocation() + (AvatarCharacter->GetActorForwardVector() * 80.f);
	// Hacemos que el proyectil mire hacia donde mira el Hechicero
	FRotator SpawnRotation = AvatarCharacter->GetActorForwardVector().Rotation();
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	// 2. Iniciar el Spawn Diferido (pausado)
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		AvatarCharacter,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (Projectile)
	{
		// 3. Crear el paquete de daño (Spec)
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddInstigator(AvatarCharacter, AvatarCharacter); // Decimos que el Hechicero es el culpable

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.f, EffectContext);
		
		// 4. Entregar el daño al proyectil
		Projectile->DamageEffectSpecHandle = SpecHandle;

		// 5. Finalizar el Spawn (el proyectil empieza a volar aquí)
		Projectile->FinishSpawning(SpawnTransform);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}