// Copyright 2021 Chris Worcester All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SeedPickup.generated.h"

class AGardeningCharacter;
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

	int32 GiveSeeds(AGardeningCharacter* Character, int32 SeedsToGive);

	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UStaticMeshComponent* TrunkMesh;

	UPROPERTY()
	UStaticMeshComponent* LeavesMesh;

	UPROPERTY()
	UCapsuleComponent* Trigger;

	UPROPERTY(Category = "Effects", EditDefaultsOnly)
	USoundBase* Rustle;

	UPROPERTY(Category = "Pickup", EditDefaultsOnly)
	int32 SeedsPerPickup = 3;

	UPROPERTY(Category = "Pickup", EditDefaultsOnly)
	float ResetDelay = 5.f;

	FTimerHandle ResetTimer;

	bool bIsActive = true;
};
