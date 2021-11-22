// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathFloor.h"

// Sets default values
ADeathFloor::ADeathFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	VisibleComponent->SetupAttachment(RootComponent);
	TriggerArea->SetupAttachment(VisibleComponent);

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ADeathFloor::OnComponentBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &ADeathFloor::OnComponentEndOverlap);
}

// Called when the game starts or when spawned
void ADeathFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeathFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathFloor::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor == Target) {
		OtherActor->Destroy();
	}
}

void ADeathFloor::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}