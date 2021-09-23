// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "GardeningPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GARDENING_API AGardeningPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	////////// FUNCTIONS //////////
	virtual void FirePressed();

	virtual void FireReleased();

	virtual void SwitchTool();

protected:
	////////// FUNCTIONS //////////
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	////////// PROPERTIES //////////
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HudClass;

	UPROPERTY()
	UUserWidget* Hud;

private:
	UPROPERTY()
	AGardeningCharacter* ControlledCharacter;
};
