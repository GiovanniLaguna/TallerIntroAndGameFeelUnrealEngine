#include "Actor/AuraWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"

AAuraWeapon::AAuraWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->bAutoActivate = false;
	
	bReplicates = true;
}

void AAuraWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraWeapon::OnSphereOverlap);
}

void AAuraWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Add rotation if thrown
	if (bIsThrown)
	{
		AddActorLocalRotation(FRotator(0.f, 1500.f * DeltaTime, 0.f));
	}
}

void AAuraWeapon::PickUp(AAuraCharacterBase* NewOwner)
{
	if (!NewOwner || bIsEquipped) return;

	CurrentOwnerCharacter = NewOwner;
	bIsEquipped = true;
	bIsThrown = false;
	
	ProjectileMovement->Deactivate();
	ProjectileMovement->Velocity = FVector::ZeroVector;

	// Attach to socket
	AttachToComponent(CurrentOwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponHandSocket"));
}

void AAuraWeapon::ThrowWeapon(FVector Direction)
{
	if (!bIsEquipped) return;

	bIsEquipped = false;
	bIsThrown = true;
	CurrentOwnerCharacter = nullptr;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = Direction * 1500.f;
}

void AAuraWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsThrown) return;
	if (OtherActor == CurrentOwnerCharacter || OtherActor == this) return;

	if (DamageEffectClass)
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor))
		{
			// Try to get instigator ASC if we stored it, else just apply directly
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			ContextHandle.AddInstigator(this, this);
			
			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.f, ContextHandle);
			if (SpecHandle.IsValid())
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	// Stop after hitting something
	bIsThrown = false;
	ProjectileMovement->Deactivate();
	ProjectileMovement->Velocity = FVector::ZeroVector;
}
