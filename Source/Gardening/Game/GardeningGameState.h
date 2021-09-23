// Fill out your copyright notice in the Description page of Project Settings.

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