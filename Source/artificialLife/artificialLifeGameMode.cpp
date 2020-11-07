// Copyright Epic Games, Inc. All Rights Reserved.

#include "artificialLifeGameMode.h"
#include "artificialLifeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AartificialLifeGameMode::AartificialLifeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		//HUDClass = AplayerHUD::StaticClass();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameMode!"));
	}

	//HUDClass = AplayerHUD::StaticClass();

}
