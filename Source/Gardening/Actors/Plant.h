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

	/* This is used to ensure references are valid before performing the dependent setup*/
	UFUNCTION()
	void SecondarySetup(const int32 TeamNumber);

	virtual void Tick(float DeltaTime) override;

	void StartGrowing();

	void StopGrowing();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	////////// PROPERTIES //////////
	UPROPERTY(BlueprintReadOnly)
	float GrowthProgress = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float GrowthTimelineLength;

	float CutDown();

	int32 TeamNumber = -1; // Initialize with invalid team so we know when it hasn't been set

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

	bool AddHeight(const float FeetToAdd);

	void DestroySelf();

	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
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

	FVector InitialScale;

	bool bGrowingSoundPlayed = false;

	const float ScaleToFeetMultiplier = 5.16f;

	float HeightInFeet = 0.f;

	FTimerHandle DestroyTimerHandle;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly)
	float Health;

	UPROPERTY()
	class AGardeningGameState* GameState;
};