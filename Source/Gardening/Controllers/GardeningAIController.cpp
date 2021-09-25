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

	FindPlayerCharacter();
	FindNearestPlant(true);
	FindNearestPlant(false);
}

void AGardeningAIController::FindPlayerCharacter()
{
	if (LineOfSightTo(PlayerPawn))
	{
		if (AICharacter->GetDistanceTo(PlayerPawn) < PlayerSearchRange)
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
}

void AGardeningAIController::FindNearestPlant(const bool FindingPlayerPlant)
{
	float SearchRange;
	TArray<AActor*> PlantsList;
	FName PlantLocationKeyName;

	if (FindingPlayerPlant)
	{
		SearchRange = PlayerPlantSearchRange;
		PlantsList = GameState->PlayerPlants;
		PlantLocationKeyName = FName("PlayerPlantLocation");
	}
	else
	{
		SearchRange = OwnPlantSearchRange;
		PlantsList = GameState->AIPlants;
		PlantLocationKeyName = FName("AIPlantLocation");
	}

	float NearestPlantDistance = SearchRange;
	FVector NearestPlantLocation;

	for (AActor* PlantActor : PlantsList)
	{
		//	if (LineOfSightTo(PlayerPlant)) // TODO Only set if the AI can see the plant
		//	{								// TODO and if the plant is high enough to cut down
		const float DistanceToPlant = AICharacter->GetDistanceTo(PlantActor);

		if (DistanceToPlant < NearestPlantDistance)
		{
			NearestPlantDistance = DistanceToPlant;
			NearestPlantLocation = PlantActor->GetActorLocation();
		}
		//	}
	}

	if (NearestPlantDistance < SearchRange)
	{
		GetBlackboardComponent()->SetValueAsVector(PlantLocationKeyName, NearestPlantLocation);
	}
	else
	{
		GetBlackboardComponent()->ClearValue(PlantLocationKeyName);
	}
}
