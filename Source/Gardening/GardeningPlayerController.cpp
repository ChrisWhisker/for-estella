// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningPlayerController.h"
#include "Blueprint/UserWidget.h"

void AGardeningPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!HudClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hud class is not set on the player controller."));
		return;
	}

	Hud->AddToViewport();
}
