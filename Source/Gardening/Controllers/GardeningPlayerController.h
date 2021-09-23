// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GardeningPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GARDENING_API AGardeningPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	////////// PROPERTIES //////////
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HudClass;

	UPROPERTY()
	UUserWidget* Hud;
};
