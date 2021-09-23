// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void AGardeningAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* AIPawn = GetPawn();
	AICharacter = Cast<AGardeningCharacter>(AIPawn);


	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (AIBehaviorTree != nullptr)
	{
		RunBehaviorTree(AIBehaviorTree);
	}
}

void AGardeningAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LineOfSightTo(PlayerPawn))
	{
		if (AICharacter->GetDistanceTo(PlayerPawn) < AttackPlayerDistance)
		{
			GetBlackboardComponent()->SetValueAsVector("PlayerLocation", PlayerPawn->GetActorLocation());
			AICharacter->SetActiveTool(AICharacter->Tool_Axe); // TODO Shouldn't do this here, it should be in a behavior tree task
		}
	}
	else
	{
		GetBlackboardComponent()->ClearValue("PlayerLocation");
	}
}
