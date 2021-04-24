// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GardeningCharacterHelper.generated.h"

class AGardeningPlayerController;
class APlant;
class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	UGardeningCharacterHelper();

	void SwitchTool();

	void UseTool();

	void StartWatering();

	void StopWatering();

	void StopWateringOnePlant(APlant* Plant);

	void WaterPlant(APlant* PlantToWater);

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

	UPROPERTY()
	UBoxComponent* WateringTrigger;

	UPROPERTY()
	USceneComponent* WaterSpawnPoint;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	void PlantSeed(FHitResult Hit);

	void UseAxe() const;

	bool Trace(FHitResult& Hit) const;

	////////// PROPERTIES //////////
	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* WateringSound;

	UPROPERTY()
	UAudioComponent* WateringSoundComponent;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	UParticleSystem* WateringParticle;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	float WaterFadeInTime = .5f;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	float WaterFadeOutTime = 2.f;

	UPROPERTY()
	UParticleSystemComponent* WateringParticleComponent;

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
