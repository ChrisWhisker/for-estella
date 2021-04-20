// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeedPickup.generated.h"

class UCapsuleComponent;

UCLASS()
class GARDENING_API ASeedPickup : public AActor
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	ASeedPickup();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	void ResetPickup();

	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UStaticMeshComponent* TrunkMesh;

	UPROPERTY()
	UStaticMeshComponent* LeavesMesh;

	UPROPERTY()
	UCapsuleComponent* Trigger;

	UPROPERTY()
	class UGardeningCharacterHelper* Helper;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* Rustle;

	FTimerHandle ResetTimer;

	UPROPERTY(Category = "Activation", EditDefaultsOnly)
	float ResetDelay = 5.f;

	bool bIsActive = true;
};
