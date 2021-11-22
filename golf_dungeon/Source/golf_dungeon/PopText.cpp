// Fill out your copyright notice in the Description page of Project Settings.


#include "PopText.h"

APopText::APopText()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	TriggerArea->SetupAttachment(RootComponent);

	TriggerDistance = 10;
	CurrentAnimationTime = 0;
	AnimationDuration = 0.7;
	PlayAppearAnimation = false;
	PlayDisappearAnimation = false;
	VisibleInitially = false;

	GetTextRender()->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	GetTextRender()->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &APopText::OnComponentBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &APopText::OnComponentEndOverlap);
}

void APopText::BeginPlay()
{
	Super::BeginPlay();

	InitialScale = GetTextRender()->GetComponentScale();
	TriggerArea->SetWorldScale3D(FVector(TriggerDistance));
	GetTextRender()->SetVisibility(VisibleInitially);
}

void APopText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FaceTarget) return;

	if (PlayAppearAnimation) {
		PlayAnimationAppear(DeltaTime);
	} else if (PlayDisappearAnimation) {
		PlayAnimationDisappear(DeltaTime);
	} else {
		RotateToTarget();
	}
}

void APopText::OnComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor == FaceTarget && !PlayDisappearAnimation) {
		PlayAppearAnimation = true;
	}
}

void APopText::OnComponentEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherActor == FaceTarget && !PlayAppearAnimation) {
		PlayDisappearAnimation = true;
	}
}

void APopText::RotateToTarget()
{
	FVector CurrentLocation = GetTextRender()->GetComponentLocation();
	FVector TargetLocation = FaceTarget->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	GetTextRender()->SetWorldRotation(FRotator(0, LookAtRotation.Yaw, LookAtRotation.Roll));
}

void APopText::PlayAnimationAppear(float DeltaTime)
{
	CurrentAnimationTime += DeltaTime;
	float RotationDegrees = 360.0f / AnimationDuration * DeltaTime;
	float ScaleFactor = FMath::Max(0.1f, FMath::Min(CurrentAnimationTime / AnimationDuration, 1.0f));

	GetTextRender()->SetVisibility(true);
	GetTextRender()->SetWorldRotation(GetActorRotation().Add(0, 0, RotationDegrees));
	GetTextRender()->SetWorldScale3D(FVector(InitialScale.X, InitialScale.Y, InitialScale.Z) * ScaleFactor);
	if (CurrentAnimationTime >= AnimationDuration) {
		PlayAppearAnimation = false;
		CurrentAnimationTime = 0;
		TriggerArea->SetWorldScale3D(FVector(TriggerDistance));
	}
}

void APopText::PlayAnimationDisappear(float DeltaTime)
{
	CurrentAnimationTime += DeltaTime;
	float RotationDegrees = -360.0f / AnimationDuration * DeltaTime;
	float ScaleFactor = FMath::Max(1 - CurrentAnimationTime / AnimationDuration, 0.1f);

	GetTextRender()->SetVisibility(true);
	GetTextRender()->SetWorldRotation(GetActorRotation().Add(0, 0, RotationDegrees));
	GetTextRender()->SetWorldScale3D(FVector(InitialScale.X, InitialScale.Y, InitialScale.Z) * ScaleFactor);
	if (CurrentAnimationTime >= AnimationDuration) {
		PlayDisappearAnimation = false;
		CurrentAnimationTime = 0;
		TriggerArea->SetWorldScale3D(FVector(TriggerDistance));
		GetTextRender()->SetVisibility(false);
	}
}