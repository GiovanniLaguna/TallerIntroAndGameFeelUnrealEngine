#include "AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. Configurar la esfera para que detecte colisiones
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Choca con el jugador
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Choca con las paredes del nivel

	// 2. Configurar el vuelo del proyectil
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 600.f; // Velocidad moderada para que el jugador pueda esquivarlo
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->ProjectileGravityScale = 0.f; // 0 significa que vuela recto, sin caer al piso
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Conectamos la función al evento de colisión
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	// Se destruye solo después de 15 segundos si falla el tiro y vuela al vacío
	SetLifeSpan(15.f);
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Evitamos que el Hechicero se haga daño a sí mismo con su propia bala
	if (OtherActor == GetInstigator() || OtherActor == this) return;

	// Si el proyectil trae un efecto de daño, lo aplicamos
	if (DamageEffectSpecHandle.IsValid())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
	}

	// Destruimos el proyectil al impactar
	Destroy();
}