// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GardeningCharacterHelper.generated.h"

class AGardeningPlayerController;
class APlant;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	UGardeningCharacterHelper();

	void SwitchTool();

	void UseTool();

	void WaterPlant(APlant* PlantToWater);

	void StopWatering();

	UFUNCTION(BlueprintGetter)
	int32 GetMaxSeeds() const;

	////////// PROPERTIES //////////
	UPROPERTY(BlueprintReadOnly)
	int32 ActiveTool = 0;

	// Effectively constants
	UPROPERTY(BlueprintReadOnly)
	FString Tool_Seeds = TEXT("Seeds");
	UPROPERTY(BlueprintReadOnly)
	FString Tool_Axe = TEXT("Axe");

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Tools = {Tool_Seeds, Tool_Axe};

	UPROPERTY(BlueprintReadWrite)
	int32 SeedCount;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	void PlantSeed(FHitResult Hit);

	void UseAxe() const;

	bool Trace(FHitResult& Hit) const;

	////////// PROPERTIES //////////
	UPROPERTY(Category = "References", EditDefaultsOnly)
	TSubclassOf<class APlant> PlantBpClass;

	UPROPERTY()
	AGardeningPlayerController* GardeningPlayerController;

	UPROPERTY(BlueprintReadOnly)
	TArray<APlant*> WateredPlants;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxSeeds = 10;

	UPROPERTY(Category = "Raycasting", EditDefaultsOnly)
	float MaxRange = 500.f;
};
