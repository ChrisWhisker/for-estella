// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "Axe.generated.h"

class AGardeningCharacter;
class UBoxComponent;

UCLASS()
class GARDENING_API AAxe : public ATool
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	AAxe();

	void SecondarySetup(AGardeningCharacter* Char);

	////////// PROPERTIES //////////
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* Trigger;

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	////////// PROPERTIES //////////
	UPROPERTY()
	AGardeningCharacter* Character;

	float DamagePerStrike = 51;

	UPROPERTY(BlueprintReadWrite)
	bool bAxeHeadActive = false; // whether or not the axe will currently cause damage
};
