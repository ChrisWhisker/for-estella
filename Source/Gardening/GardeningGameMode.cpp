// Copyright Epic Games, Inc. All Rights Reserved.

#include "GardeningGameMode.h"
#include "Pawns/GardeningCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGardeningGameMode::AGardeningGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
