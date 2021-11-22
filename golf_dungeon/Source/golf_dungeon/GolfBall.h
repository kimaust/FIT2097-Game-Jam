// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "golf_dungeonGameMode.h"
#include "GolfBall.generated.h"

USTRUCT()
struct FProjectilePathInformation {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float ProjectileRadius;

	UPROPERTY(EditAnywhere)
	float SimulationFrequency;
};

UCLASS()
class GOLF_DUNGEON_API AGolfBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGolfBall();

	bool IsAttacking();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* VisibleComponent;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	bool IsCharging;
	bool CanLaunch;
	float LaunchSpeed;

	UPROPERTY(EditAnywhere)
	float CharingSpeed;

	UPROPERTY(EditAnywhere)
	float MaxLaunchSpeed;

	UPROPERTY(EditAnywhere)
	float SnapTolerance;

	UPROPERTY(EditAnywhere)
	float LinearDamping;

	UPROPERTY(EditAnywhere)
	float AngularDamping;

	UPROPERTY(EditAnywhere)
	float SpringArmLength;

	UPROPERTY(EditAnywhere)
	FVector CameraOffset;

	UPROPERTY(EditAnywhere)
	FProjectilePathInformation ProjectilePathInformation;

	UPROPERTY(EditAnywhere)
	USoundBase* JumpSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* JumpEffect;

	Agolf_dungeonGameMode* GameMode;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Camera movement
	virtual void Turn(float AxisValue);
	virtual void LookUp(float AxisValue);
	virtual void Zoom(float AxisValue);

	// Ball mechanic
	virtual void Charge();
	virtual void Discharge();

	virtual bool OnGround();
	virtual FVector GetLaunchVector();
	virtual void DrawProjectilePath();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
};
