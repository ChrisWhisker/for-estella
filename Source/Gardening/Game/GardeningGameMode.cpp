// Copyright Epic Games, Inc. All Rights Reserved.

#include "GardeningGameMode.h"
#include "Gardening/Character/GardeningCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGardeningGameMode::AGardeningGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Gardening/Blueprints/BP_GardeningCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
