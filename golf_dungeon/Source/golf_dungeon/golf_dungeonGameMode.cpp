// Copyright Epic Games, Inc. All Rights Reserved.

#include "golf_dungeonGameMode.h"
#include "golf_dungeonHUD.h"
#include "golf_dungeonCharacter.h"
#include "UObject/ConstructorHelpers.h"

Agolf_dungeonGameMode::Agolf_dungeonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	CurrentScore = 0;
	CurrentLaunchSpeed = 0;
	CurrentMaxLaunchSpeed = 5000;
}

int Agolf_dungeonGameMode::GetCurrentScore()
{
	return CurrentScore;
}

void Agolf_dungeonGameMode::IncrementScore() 
{
	++CurrentScore;
}

float Agolf_dungeonGameMode::GetLaunchSpeedPercentage()
{
	return CurrentLaunchSpeed / CurrentMaxLaunchSpeed;
}

void Agolf_dungeonGameMode::SetLaunchSpeed(float LaunchSpeed)
{
	CurrentLaunchSpeed = LaunchSpeed;
}

void Agolf_dungeonGameMode::SetMaxLaunchSpeed(float MaxLaunchSpeed)
{
	CurrentMaxLaunchSpeed = MaxLaunchSpeed;
}