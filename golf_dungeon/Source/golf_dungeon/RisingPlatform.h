// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RisingPlatform.generated.h"

UCLASS()
class GOLF_DUNGEON_API ARisingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARisingPlatform();

	UPROPERTY(EditAnywhere)
	float TravelDistance;
	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(EditAnywhere)
	float Tolerance; //how close is close enough to the target position?
	UStaticMeshComponent* VisibleComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector StartingPosition;
	float Direction;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
