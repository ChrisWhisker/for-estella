// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardeningCharacterHelper.generated.h"

class AGardeningPlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float MaxRange = 500.f;

	UPROPERTY(EditAnywhere, Category = "References", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlant> PlantBpClass;

	APlant* PlantSeed(FHitResult Hit);
	void WaterPlant(FHitResult Hit);
	void UseAxe();

	UPROPERTY()
	AGardeningPlayerController* GardeningPlayerController;

public:
	UGardeningCharacterHelper();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void UseTool();
	void StopUsingTool();
	void SwitchTool(int32 NewToolIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ActiveTool = 0;

	// Effectively constants
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Tool_Seeds = TEXT("Seeds");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Tool_WateringCan = TEXT("Watering Can");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Tool_Axe = TEXT("Axe");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> Tools = {Tool_Seeds, Tool_WateringCan, Tool_Axe};

	UFUNCTION(BlueprintGetter)
	int32 GetMaxSeeds();

	UFUNCTION(BlueprintGetter)
	int32 GetSeedCount();

	UFUNCTION(BlueprintSetter)
	void SetSeedCount(int32 NewSeedCount);

protected:
	virtual void BeginPlay() override;
	bool Trace(FHitResult& Hit);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlant* WateredPlant;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxSeeds = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 SeedCount;
};
