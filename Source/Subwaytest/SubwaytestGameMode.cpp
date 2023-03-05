// Copyright Epic Games, Inc. All Rights Reserved.

#include "SubwaytestGameMode.h"
#include "SubwaytestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASubwaytestGameMode::ASubwaytestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
