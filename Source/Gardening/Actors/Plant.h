// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

class UCurveFloat;

UCLASS()
class GARDENING_API APlant : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category="Growth")
	UCurveFloat* GrowthCurve;

	UPROPERTY()
	FVector StartScale;

	UPROPERTY(EditDefaultsOnly, Category = "Growth")
	FVector MaxScale = FVector(1, 1, 2);

public:
	APlant();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void StartGrowing();

	UFUNCTION()
	void StopGrowing();
};
