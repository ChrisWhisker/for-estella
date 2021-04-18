// Fill out your copyright notice in the Description page of Project Settings.


#include "GardeningPlayerController.h"
#include "Blueprint/UserWidget.h"

void AGardeningPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetUpHud();
	SetUpGrowthWidget();
}

void AGardeningPlayerController::SetUpHud()
{
	if (!HudClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hud class is not set on the player controller."));
		return;
	}

	Hud = CreateWidget(this, HudClass);
	if (!Hud)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hud is not being created on the player controller."));
		return;
	}
	Hud->AddToViewport();
}

void AGardeningPlayerController::SetUpGrowthWidget()
{
	if (!GrowthWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Growth Widget class is not set on the player controller."));
		return;
	}

	GrowthWidget = CreateWidget(this, GrowthWidgetClass);
	if (!GrowthWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Growth Widget is not being created on the player controller."));
		return;
	}
}