// Copyright 2021 Chris Worcester All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GardeningGameState.generated.h"

/**
 * 
 */
UCLASS()
class GARDENING_API AGardeningGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	UFUNCTION(BlueprintImplementableEvent)
	void AddGardenHeightFeet(int32 TeamNumber, float FeetToAdd);

	UFUNCTION(BlueprintImplementableEvent)
	void AddPlantToGarden(int32 TeamNumber, class APlant* Plant);

	UFUNCTION(BlueprintImplementableEvent)
	void RemovePlantFromGarden(int32 TeamNumber, class APlant* Plant);

	////////// PROPERTIES //////////
	UPROPERTY(BlueprintReadWrite, Category="Player")
	TArray<AActor*> PlayerPlants;

	UPROPERTY(BlueprintReadWrite, Category="Player")
	float PlayerGardenHeightFeet = 0.f;

	UPROPERTY(BlueprintReadWrite, Category="AI")
	TArray<AActor*> AIPlants;

	UPROPERTY(BlueprintReadWrite, Category="AI")
	float AIGardenHeightFeet = 0.f;
};
