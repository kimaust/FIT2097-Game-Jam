// Fill out your copyright notice in the Description page of Project Settings.


#include "RisingPlatform.h"

// Sets default values
ARisingPlatform::ARisingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);

	//Sets a default mesh for the object
	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshObj(TEXT("/Game/Geometry/Meshes/1M_Cube.1M_Cube"));

	if (CubeMeshObj.Succeeded()) {
		VisibleComponent->SetStaticMesh(CubeMeshObj.Object);
	}

	VisibleComponent->SetRelativeScale3D({2.5, 2.5, 0.5});

	TravelDistance = 2000;
	Speed = 200;
	Tolerance = 10;
	Direction = 1;

}

// Called when the game starts or when spawned
void ARisingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartingPosition = GetActorLocation();
	
}

// Called every frame
void ARisingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector TargetDirection = GetActorUpVector() * Direction;
	FVector CurrentPosition = GetActorLocation();

	// We calculate target position here instead. Ultimately little difference between this and World Actor

	FVector TargetPosition = StartingPosition + (TargetDirection * TravelDistance);

	//Increment Position
	CurrentPosition += TargetDirection * Speed * DeltaTime;

	//Same as World Actor
	if (FVector::Dist(CurrentPosition, TargetPosition) <= Tolerance) {
		CurrentPosition = TargetPosition;
		StartingPosition = CurrentPosition;
		Direction *= -1;
	}

	SetActorLocation(CurrentPosition);
}
