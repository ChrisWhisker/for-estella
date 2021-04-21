// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

class UCurveFloat;
class UWidgetComponent;

UCLASS()
class GARDENING_API APlant : public AActor
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	APlant();

	virtual void Tick(float DeltaTime) override;

	void StartGrowing();

	void StopGrowing();

	////////// PROPERTIES //////////
	UPROPERTY(BlueprintReadOnly)
	float GrowthProgress = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float GrowthTimelineLength;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* Rustle;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* PlantingSound;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	UParticleSystem* PlantingParticle;

	UPROPERTY(Category="Growth", EditDefaultsOnly)
	UCurveFloat* GrowthCurve;

	UPROPERTY(Category="Growth", EditDefaultsOnly)
	FVector MaxScale = FVector(1, 1, 2);

	UPROPERTY()
	UWidgetComponent* ProgressBarWidget;

	FTimeline GrowthTimeline;

	FVector StartScale;

	bool bGrowingSoundPlayed = false;
};
