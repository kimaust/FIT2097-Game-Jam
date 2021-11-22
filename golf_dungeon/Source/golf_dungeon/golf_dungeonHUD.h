// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "golf_dungeonHUD.generated.h"

UCLASS()
class Agolf_dungeonHUD : public AHUD
{
	GENERATED_BODY()

public:
	Agolf_dungeonHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

