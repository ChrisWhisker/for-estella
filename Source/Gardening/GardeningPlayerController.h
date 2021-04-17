// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GardeningPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GARDENING_API AGardeningPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ManageGrowthWidget(bool bAdding);
	
protected:
	virtual void BeginPlay() override;
	void SetUpHud();
	void SetUpGrowthWidget();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HudClass;

	UPROPERTY()
	UUserWidget* Hud;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GrowthWidgetClass;

	UPROPERTY()
	UUserWidget* GrowthWidget;
};
