// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "golf_dungeonGameMode.generated.h"

UCLASS(minimalapi)
class Agolf_dungeonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	Agolf_dungeonGameMode();

	UFUNCTION(BlueprintPure)
	int GetCurrentScore();

	UFUNCTION(BlueprintCallable)
	void IncrementScore();

	UFUNCTION(BlueprintPure)
	float GetLaunchSpeedPercentage();

	UFUNCTION(BlueprintCallable)
	void SetLaunchSpeed(float NewLaunchSpeed);

	UFUNCTION(BlueprintCallable)
	void SetMaxLaunchSpeed(float MaxLaunchSpeed);
protected:
	int CurrentScore;
	float CurrentLaunchSpeed;
	float CurrentMaxLaunchSpeed;
};



