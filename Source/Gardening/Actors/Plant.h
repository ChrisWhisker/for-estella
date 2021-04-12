// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plant.generated.h"

UCLASS()
class GARDENING_API APlant : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Growth")
	float MaxScale = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Growth")
	float GrowSpeed = 1.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

public:
	// Sets default values for this actor's properties
	APlant();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// void Grow();
};
