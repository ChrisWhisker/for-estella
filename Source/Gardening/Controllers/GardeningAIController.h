// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Gardening/Character/GardeningCharacter.h"

#include "GardeningAIController.generated.h"

/**
 * 
 */
UCLASS()
class GARDENING_API AGardeningAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	APawn* PlayerPawn;

	// UPROPERTY(EditAnywhere)
	// float AcceptanceRadius = 200;

	UPROPERTY(EditAnywhere)
	float AttackPlayerDistance = 10000;

	UPROPERTY()
	AGardeningCharacter* AICharacter;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviorTree;
};
