// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GardeningCharacterHelper.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GARDENING_API UGardeningCharacterHelper : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGardeningCharacterHelper();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void UseItem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool Trace(FHitResult& Hit, FVector& ShotDirection);

private:
	UPROPERTY(EditAnywhere)
	float MaxRange = 500.f;

	// Step 1. Declare a property of type blueprint
	UPROPERTY(EditAnywhere, /*Category = Players,*/ meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APlant> PlantBpClass;
 
	// Step 2. Wherever you need a UClass * reference for that blueprint (like SpawnActor)
	// AActor *player = GetWorld()->SpawnActor(PlayerBlueprintType.Get(), &playerStart->GetActorTransform(), params);
};
