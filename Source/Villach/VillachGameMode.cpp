// Copyright Epic Games, Inc. All Rights Reserved.

#include "VillachGameMode.h"
#include "VillachCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVillachGameMode::AVillachGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
