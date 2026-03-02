#include "AuraHazardActor.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

AAuraHazardActor::AAuraHazardActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. Creamos la caja de daño
	DamageZone = CreateDefaultSubobject<UBoxComponent>("DamageZone");
	SetRootComponent(DamageZone);
	
	// Le damos un tamaño inicial decente
	DamageZone->SetBoxExtent(FVector(100.f, 100.f, 50.f)); 
	DamageZone->SetCollisionResponseToAllChannels(ECR_Overlap);

	bIsActive = false; // Empieza apagada
}

void AAuraHazardActor::BeginPlay()
{
	Super::BeginPlay();

	DamageZone->OnComponentBeginOverlap.AddDynamic(this, &AAuraHazardActor::OnOverlap);

	// 2. Arrancamos el temporizador infinito
	GetWorld()->GetTimerManager().SetTimer(HazardTimer, this, &AAuraHazardActor::ToggleHazard, InactiveTime, false);

	// Le decimos al Blueprint cuál es el estado inicial
	OnHazardStateChanged(bIsActive);
}

void AAuraHazardActor::ToggleHazard()
{
	// 3. Invertimos el estado (Si estaba apagada se enciende, y viceversa)
	bIsActive = !bIsActive;
	// Le avisamos al Blueprint que el estado acaba de cambiar
	OnHazardStateChanged(bIsActive);

	// Calculamos cuánto tiempo falta para el próximo cambio
	float NextDelay = bIsActive ? ActiveTime : InactiveTime;

	// Volvemos a programar el reloj
	GetWorld()->GetTimerManager().SetTimer(HazardTimer, this, &AAuraHazardActor::ToggleHazard, NextDelay, false);
}

void AAuraHazardActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 4. Si la trampa está APAGADA, no hacemos nada
	if (!bIsActive) return;

	// 5. Si está ENCENDIDA, aplicamos el daño (Igual que la poción, pero con daño)
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		UAbilitySystemComponent* TargetASC = ASCInterface->GetAbilitySystemComponent();
		
		if (DamageEffectClass && TargetASC)
		{
			FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			
			FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);
			TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		}
	}
}