// Fill out your copyright notice in the Description page of Project Settings.

#include "golf_dungeonTarget.h"

// Sets default values
Agolf_dungeonTarget::Agolf_dungeonTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the model
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	VisibleModel->SetupAttachment(RootComponent);
	TriggerArea->SetupAttachment(VisibleModel);

	ConstructorHelpers::FObjectFinder<UStaticMesh> VisibleModelAsset(TEXT("/Game/Assets/targetModel.targetModel"));
	if (VisibleModelAsset.Succeeded())
	{
		VisibleModel->SetStaticMesh(VisibleModelAsset.Object);
	}
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &Agolf_dungeonTarget::OnComponentBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &Agolf_dungeonTarget::OnComponentEndOverlap);
}

// Called when the game starts or when spawned
void Agolf_dungeonTarget::BeginPlay()
{
	Super::BeginPlay();

	const float TriggerAreaScaleFactor = 0.1f;
	TriggerArea->SetWorldScale3D(FVector(VisibleModel->GetComponentScale() * TriggerAreaScaleFactor));
	GameMode = Cast<Agolf_dungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void Agolf_dungeonTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void Agolf_dungeonTarget::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor == GolfBall) {
		GameMode->IncrementScore();
		Destroy();
	}
}

void Agolf_dungeonTarget::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{

}