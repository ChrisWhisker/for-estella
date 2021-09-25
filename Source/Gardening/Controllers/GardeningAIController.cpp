// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Gardening/Game/GardeningGameState.h"
#include "Kismet/GameplayStatics.h"

void AGardeningAIController::BeginPlay()
{
	Super::BeginPlay();

	APawn* AIPawn = GetPawn();
	AICharacter = Cast<AGardeningCharacter>(AIPawn);

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (GetWorld() == nullptr) { UE_LOG(LogTemp, Error, TEXT("GetWorld() returned null so game state is not set.")); }
	else
	{
		GameState = GetWorld()->GetGameState<AGardeningGameState>();
	}

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
			AICharacter->SetActiveTool(AICharacter->Tool_Axe);
			// TODO Shouldn't do this here, it should be in a behavior tree task
		}
	}
	else
	{
		GetBlackboardComponent()->ClearValue("PlayerLocation");
	}

	float NearestPlayerPlantDistance = AttackPlayerPlantDistance;
	FVector NearestPlayerPlantLocation;

	for (AActor* PlayerPlant : GameState->PlayerPlants)
	{
		//	if (LineOfSightTo(PlayerPlant)) // TODO Only set if the AI can see the plant
		//	{
		const float DistanceToPlant = AICharacter->GetDistanceTo(PlayerPlant);

		if (DistanceToPlant < NearestPlayerPlantDistance)
		{
			NearestPlayerPlantDistance = DistanceToPlant;
			NearestPlayerPlantLocation = PlayerPlant->GetActorLocation();
		}
		//	}
	}

	if (NearestPlayerPlantDistance < AttackPlayerPlantDistance)
	{
		GetBlackboardComponent()->SetValueAsVector("PlayerPlantLocation", NearestPlayerPlantLocation);
	}
	else
	{
		GetBlackboardComponent()->ClearValue("PlayerPlantLocation");
	}
}
