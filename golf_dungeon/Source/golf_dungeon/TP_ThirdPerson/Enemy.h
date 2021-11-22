// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TP_ThirdPersonCharacter.h"
#include "../GolfBall.h"
#include "AIController.h"
#include "AITypes.h"
#include "Particles/ParticleSystem.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class GOLF_DUNGEON_API AEnemy : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()
	
public:
	//Default Constructor
	AEnemy();

	UPROPERTY(EditAnywhere)
	int RoamRadius;

	UPROPERTY(EditAnywhere)
	float RoamDelay;

	UPROPERTY(EditAnywhere)
	float AttackRadius;

	//How close the enemy and the ball have to be in order for them to be "colliding".
	UPROPERTY(EditAnywhere)
	float CollidingTolerance;

	UPROPERTY(EditAnywhere)
	AGolfBall* Target;

	UPROPERTY(EditAnywhere)
	UParticleSystem* DestroyEffect;

	void Roam();
	void Chase();
	
protected:
	//A bool containing what state the enemy is in. True means Attack Mode. False means Roam Mode.
	bool IsRoaming;
	float ElapsedTime;
	FVector HomeLocation;

	bool IsFacingPlayer();
	bool CollisionCheck();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
