// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "PopText.generated.h"

/**
 *
 */
UCLASS()
class GOLF_DUNGEON_API APopText : public ATextRenderActor
{
	GENERATED_BODY()
public:
	APopText();

	UFUNCTION()
	void OnComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	virtual void RotateToTarget();
	virtual void PlayAnimationAppear(float DeltaTime);
	virtual void PlayAnimationDisappear(float DeltaTime);

	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere)
	float TriggerDistance;

	UPROPERTY(EditAnywhere)
	AActor* FaceTarget;

	UPROPERTY(EditAnywhere)
	bool VisibleInitially;

	bool PlayAppearAnimation;
	bool PlayDisappearAnimation;
	float CurrentAnimationTime;
	FVector InitialScale;

	UPROPERTY(EditAnywhere)
	float AnimationDuration;

	virtual void BeginPlay() override;
};
