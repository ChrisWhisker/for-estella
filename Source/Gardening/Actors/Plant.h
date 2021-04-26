// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

class UCurveFloat;
class UGardeningCharacterHelper;
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
	UPROPERTY()
	UGardeningCharacterHelper* CharacterHelper;

	UPROPERTY(BlueprintReadOnly)
	float GrowthProgress = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float GrowthTimelineLength;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	/* This is used to ensure references are valid before performing the dependent setup*/
	UFUNCTION()
	void InitialSetupDelayed() const;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* RustleSound;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* PlantingSound;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	UParticleSystem* PlantingParticle;

	UPROPERTY(Category="Growth", EditDefaultsOnly)
	UCurveFloat* GrowthCurve;

	UPROPERTY(Category="Growth", EditDefaultsOnly, meta = (AllowPreserveRatio))
	FVector MaxScale = FVector(1, 1, 2);

	/* Maximum plant height can vary by this percent of MaxScale.Z */
	UPROPERTY(Category="Growth", EditDefaultsOnly)
	float MaxHeightVariance = 0.2f;

	UPROPERTY()
	UWidgetComponent* ProgressBarWidget;

	FTimeline GrowthTimeline;

	FVector StartScale;

	bool bGrowingSoundPlayed = false;

	const float ScaleToFeetMultiplier = 5.16f;
};
