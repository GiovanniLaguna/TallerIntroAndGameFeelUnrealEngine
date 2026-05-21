// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraEnemy.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include  "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	// Le decimos que siempre tome su cerebro, ya sea puesto a mano o por Spawner
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Asegúrate de tener el include arriba: #include "AbilitySystemComponent.h"

	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent()) // O tu variable del ASC
	{
		// Recorremos la lista y le damos cada habilidad
		for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
		{
			if (AbilityClass)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

void AAuraEnemy::FireRangedAttack()
{
	// Buscamos el sistema de habilidades del enemigo
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this))
	{
		// Le decimos que active la habilidad con la etiqueta del disparo
		// (Asegúrate de que este tag sea exactamente el mismo que pusiste en tu GA_HechiceroDisparo)
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.RangedDamage")));
        
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void AAuraEnemy::FireMeleeAttack()
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(this))
	{
		FGameplayTagContainer TagContainer;
		// Esta etiqueta es exclusiva para los golpes físicos
		TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.MeleeAttack")));
        
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

#include "Camera/CameraComponent.h"
#include "FCTween.h"

void AAuraEnemy::Die()
{
	Super::Die();

	// Ejecutar Screen Shake mediante código usando Fresh Baked Tweens (FCTween)
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* PlayerPawn = PC->GetPawn())
		{
			if (UCameraComponent* CameraComp = PlayerPawn->FindComponentByClass<UCameraComponent>())
			{
				FVector OriginalLoc = CameraComp->GetRelativeLocation();
				
				// Hacemos un tween de 0 a 1 durante 0.35 segundos
				FCTweenInstanceFloat* Tween = FCTween::Play(0.0f, 1.0f, [CameraComp, OriginalLoc](float t)
				{
					if (IsValid(CameraComp))
					{
						// (1.0 - t) asegura que el temblor pierda fuerza con el tiempo
						float Intensity = (1.0f - t) * 30.0f;
						FVector RandomOffset = FVector(
							FMath::RandRange(-Intensity, Intensity),
							FMath::RandRange(-Intensity, Intensity),
							FMath::RandRange(-Intensity, Intensity)
						);
						CameraComp->SetRelativeLocation(OriginalLoc + RandomOffset);
					}
				}, 0.35f, EFCEase::OutQuad);

				// Cuando el tween termine, regresamos la cámara a su posición original intacta
				Tween->SetOnComplete([CameraComp, OriginalLoc]()
				{
					if (IsValid(CameraComp))
					{
						CameraComp->SetRelativeLocation(OriginalLoc);
					}
				});
			}
		}
	}

	PlayDeathShake(); // Aún mantenemos el evento de Blueprint por si acaso quieres agregar más cosas

	// Configurar desvanecimiento visual del enemigo antes de destruirse
	SetLifeSpan(3.0f); // Aseguramos que el actor vivirá exactamente 3 segundos más (Server y Cliente)

	if (DynamicMaterial)
	{
		UMaterialInstanceDynamic* Mat = DynamicMaterial;
		// Hacemos el tween del parámetro Alpha de 1.0 a 0.0 durante 2.9 segundos
		FCTween::Play(1.0f, 0.0f, [Mat](float AlphaValue)
		{
			if (IsValid(Mat))
			{
				Mat->SetScalarParameterValue(FName("DeathAlpha"), AlphaValue);
			}
		}, 2.9f, EFCEase::Linear);
	}
}