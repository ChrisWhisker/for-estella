// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

class UWidgetComponent;
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

	UPROPERTY()
	FTimeline GrowthTimeline;

	UPROPERTY(EditAnywhere, Category="Growth")
	UCurveFloat* GrowthCurve;

	UPROPERTY()
	FVector StartScale;

	UPROPERTY(EditDefaultsOnly, Category = "Growth")
	FVector MaxScale = FVector(1, 1, 2);

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* Rustle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* PlantingSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* PlantingParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Growth")
	float GrowthProgress = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Growth")
	float GrowthTimelineLength;

	UWidgetComponent* ProgressBarWidget;

public:
	APlant();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void StartGrowing();

	UFUNCTION()
	void StopGrowing();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	                    
	void SetProgressBarVisibility(bool bSetVisible);
};
