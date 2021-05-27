// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Bucket.generated.h"

UCLASS()
class GARDENING_API ABucket : public ATool
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	ABucket();

	void StartPouring();

	void StopPouring() const;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	////////// PROPERTIES //////////
	UPROPERTY(Category = "Components", EditDefaultsOnly)
	USceneComponent* WaterSpawnPoint;

	UPROPERTY(Category = "Components", EditAnywhere)
	UAudioComponent* WaterSoundComponent;

	UPROPERTY(Category = "Components", EditAnywhere)
	UParticleSystemComponent* WaterParticleComponent;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* WaterSound;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	UParticleSystem* WaterParticle;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	float WaterSoundFadeInSeconds = .5f;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	float WaterSoundFadeOutSeconds = 1.f;
};
