// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GardeningCharacterHelper.generated.h"

class AAxe;
class ABucket;
class AGardeningPlayerController;
class APlant;
class ASack;
class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	UGardeningCharacterHelper();

	void SwitchTool();

	void FirePressed();

	void FireReleased();

	void StartWatering();

	void StopWatering();

	void StopWateringOnePlant(APlant* Plant);

	void WaterPlant(APlant* PlantToWater);

	UFUNCTION(BlueprintGetter)
	FORCEINLINE int32 GetMaxSeeds() const { return MaxSeeds; }

    void AddFeetToGardenHeight(float Height);

	void SubtractFeetFromGardenHeight(float Height);

	////////// PROPERTIES //////////
	UPROPERTY()
	AAxe* Axe;

	UPROPERTY()
	ABucket* Bucket;

	UPROPERTY()
	ASack* Sack;

	UPROPERTY(BlueprintReadOnly)
	int32 ActiveTool = 0;

	// Effectively constants
	UPROPERTY(BlueprintReadOnly)
	FString Tool_Seeds = TEXT("Seeds");
	UPROPERTY(BlueprintReadOnly)
	FString Tool_Water = TEXT("Water");
	UPROPERTY(BlueprintReadOnly)
	FString Tool_Axe = TEXT("Axe");

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Tools = {Tool_Seeds, Tool_Water, Tool_Axe};

	UPROPERTY(BlueprintReadWrite)
	int32 SeedCount;

	UPROPERTY()
	UBoxComponent* WateringTrigger;

	UPROPERTY()
	USceneComponent* WaterSpawnPoint;

	UPROPERTY()
	USoundBase* WaterSound;

	UPROPERTY()
	UParticleSystem* WaterParticle;

	UPROPERTY()
	float WaterSoundFadeInSeconds;

	UPROPERTY()
	float WaterSoundFadeOutSeconds;

	UPROPERTY()
	float MaxTraceRange;

	bool bIsPourWaterHeld = false;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	void PlantSeed(FHitResult Hit);

	void UseAxe() const;

	bool Trace(FHitResult& Hit) const;

	////////// PROPERTIES //////////
	UPROPERTY(Category = "References", EditDefaultsOnly)
	TSubclassOf<class APlant> PlantBlueprintClass;

	UPROPERTY()
	UAudioComponent* WaterSoundComponent;

	UPROPERTY()
	UParticleSystemComponent* WateringParticleComponent;

	UPROPERTY()
	AGardeningPlayerController* GardeningPlayerController;

	UPROPERTY(BlueprintReadOnly)
	float GardenHeightFeet = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TArray<APlant*> WateredPlants;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxSeeds = 10;
};
