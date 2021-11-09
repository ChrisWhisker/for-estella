// Copyright 2021 Chris Worcester All Rights Reserved.

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
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class UUserWidget> HudClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* Hud;

private:
	UPROPERTY()
	AGardeningCharacter* ControlledCharacter;
};
