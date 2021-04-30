// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GardeningCharacterBase.h"
#include "GardeningCharacter.generated.h"

class APlant;

UCLASS(config=Game)
class AGardeningCharacter : public AGardeningCharacterBase
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	AGardeningCharacter();

	virtual void SwitchTool();

	virtual void FirePressed();

	virtual void FireReleased();

	void StartWatering();

	void StopWatering();

	void StartWateringPlant(APlant* PlantToWater);

	void StopWateringPlant(APlant* WateredPlant);

	UFUNCTION(BlueprintGetter)
	FORCEINLINE int32 GetMaxSeeds() const { return MaxSeeds; }

	void SetGardenHeightFeet(float HeightInFeet);

	UFUNCTION(BlueprintGetter)
	FORCEINLINE int32 GetGardenHeightFeet() const { return GardenHeightFeet; }

	////////// PROPERTIES //////////
	UPROPERTY(Category = "Components", EditDefaultsOnly)
	class UBoxComponent* WateringTrigger;

	UPROPERTY(BlueprintReadOnly)
	int32 ActiveToolIndex = 0;

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

	UPROPERTY(BlueprintReadOnly)
	bool bIsPourWaterHeld = false;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlantSeed(FHitResult Hit);

	void UseAxe() const;

	bool Trace(FHitResult& Hit) const;

	UFUNCTION()
	virtual void OnTriggerOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                                 class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	////////// PROPERTIES //////////
	UPROPERTY(Category = "References", EditDefaultsOnly)
	TSubclassOf<class APlant> PlantClass;

	UPROPERTY(Category = "References", EditDefaultsOnly)
	TSubclassOf<class AAxe> AxeClass;

	UPROPERTY(Category = "References", EditDefaultsOnly)
	TSubclassOf<class ABucket> BucketClass;

	UPROPERTY(Category = "References", EditDefaultsOnly)
	TSubclassOf<class ASack> SackClass;

	UPROPERTY(Category = "Raycasting", EditDefaultsOnly)
	float MaxTraceRange = 500.f;

	UPROPERTY()
	AAxe* Axe;

	UPROPERTY()
	ABucket* Bucket;

	UPROPERTY()
	ASack* Sack;

	UPROPERTY(BlueprintReadOnly)
	float GardenHeightFeet = 0.f;

	UPROPERTY(BlueprintReadOnly)
	TArray<APlant*> WateredPlants;

	const int32 MaxSeeds = 10;
};
