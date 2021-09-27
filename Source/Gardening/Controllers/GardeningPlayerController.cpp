// Copyright 2021 Chris Worcester All Rights Reserved.


#include "GardeningPlayerController.h"
#include "Blueprint/UserWidget.h"

void AGardeningPlayerController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = GetPawn();
	ControlledCharacter = Cast<AGardeningCharacter>(PlayerPawn);
	if (!ControlledCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("ControlledCharacter not available on the player controller."));
		return;
	}

	if (!HudClass)
	{
		UE_LOG(LogTemp, Error, TEXT("Hud class is not set on the player controller."));
		return;
	}

	Hud = CreateWidget(this, HudClass);
	if (!Hud)
	{
		UE_LOG(LogTemp, Error, TEXT("Hud is not being created on the player controller."));
		return;
	}
	Hud->AddToViewport();
}

void AGardeningPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Fire", IE_Pressed, this, &AGardeningPlayerController::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AGardeningPlayerController::FireReleased);
	InputComponent->BindAction("SwitchTool", IE_Released, this, &AGardeningPlayerController::SwitchTool);
}

void AGardeningPlayerController::FirePressed()
{
	ControlledCharacter->FirePressed(0);
}

void AGardeningPlayerController::FireReleased()
{
	ControlledCharacter->FireReleased();
}

void AGardeningPlayerController::SwitchTool()
{
	ControlledCharacter->SwitchTool();
}
