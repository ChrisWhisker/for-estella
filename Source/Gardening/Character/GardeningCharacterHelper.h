// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardeningCharacterHelper.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float MaxRange = 500.f;

	UPROPERTY(EditAnywhere, /*Category = Players,*/ meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlant> PlantBpClass;

	const FString Tool_Seeds = TEXT("Seeds");
	const FString Tool_WateringCan = TEXT("Watering Can");
	const FString Tool_Axe = TEXT("Axe");

public:
	UGardeningCharacterHelper();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void UseTool();
	void SwitchTool(int32 NewToolIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ActiveTool = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> Tools = {Tool_Seeds, Tool_WateringCan, Tool_Axe};

protected:
	virtual void BeginPlay() override;
	bool Trace(FHitResult& Hit, FVector& ShotDirection);
};
