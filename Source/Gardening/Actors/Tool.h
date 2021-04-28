// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UCLASS()
class GARDENING_API ATool : public AActor
{
	GENERATED_BODY()
	
public:	
	////////// FUNCTIONS //////////
	ATool();

	protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;
	
	////////// PROPERTIES //////////
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;
};
