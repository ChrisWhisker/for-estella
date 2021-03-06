// Copyright 2021 Chris Worcester All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Gardening/Character/GardeningCharacter.h"

#include "GardeningAIController.generated.h"

/**
 * 
 */
UCLASS()
class GARDENING_API AGardeningAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	///////////// PROPERTIES //////////
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	AGardeningCharacter* AICharacter;

	UPROPERTY(EditAnywhere)
	float PlayerSearchRange = 6000;

	UPROPERTY(EditAnywhere)
	float PlayerPlantSearchRange = 12000;

	UPROPERTY(EditAnywhere)
	float OwnPlantSearchRange = 8000;

	FTimerHandle PickupSearchTimerHandle;

private:
	////////// FUNCTIONS //////////
	void FindPlayerCharacter();

	void FindNearestPlant(bool FindingPlayerPlant); // true if finding player's plant, false if finding own (AI's) plant

	UFUNCTION()
	void FindNearestPickup();

	///////////// PROPERTIES //////////
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviorTree;

	UPROPERTY()
	class AGardeningGameState* GameState;
};
