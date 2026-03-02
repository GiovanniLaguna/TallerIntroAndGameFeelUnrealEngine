// Copyrigh Quetz-Lab 2026 


#include "Enemy/AIController/AuraAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AuraEnemy.h"
// Incluye aquí el header de tu enemigo base, por ejemplo: #include "AuraEnemy.h"

class AAuraEnemy;

void AAuraAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Obtenemos al enemigo que estamos controlando
	AAuraEnemy* ControlledEnemy = Cast<AAuraEnemy>(GetPawn());
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (ControlledEnemy && PlayerPawn)
	{
		// Usamos el radio que configuraste en el Blueprint del enemigo
		MoveToActor(PlayerPawn, ControlledEnemy->AttackRadius);

		float DistanceToPlayer = FVector::Dist(ControlledEnemy->GetActorLocation(), PlayerPawn->GetActorLocation());
			
		if (DistanceToPlayer <= ControlledEnemy->AttackRadius + 50.f)
		{
			// El Hechicero usará esta, el Esbirro la ignorará
			ControlledEnemy->FireRangedAttack(); 
    
			// El Esbirro usará esta, el Hechicero la ignorará
			ControlledEnemy->FireMeleeAttack(); 
		}
	}
}