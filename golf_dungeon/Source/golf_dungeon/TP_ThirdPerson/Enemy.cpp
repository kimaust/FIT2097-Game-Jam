// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    // RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    // VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
    // VisibleComponent->SetupAttachment(RootComponent);

    RoamRadius = 1500;
    AttackRadius = 2000;
    CollidingTolerance = 150;
    ElapsedTime = 0;
    RoamDelay = 3;
    IsRoaming = false;
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    HomeLocation = GetActorLocation();
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
   
    if (!Target) return;

    if (IsRoaming) ElapsedTime += DeltaTime;
    if (ElapsedTime >= RoamDelay) {
        ElapsedTime = 0;
        IsRoaming = false;
    }

    if (CollisionCheck()) {
        if (Target->IsAttacking()) {
            Destroy();
        } else {
            Target->Destroy();
        }
        if (DestroyEffect) {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(), DestroyEffect, GetActorLocation(), FRotator::ZeroRotator, true);
        }
    }
    if (!IsFacingPlayer() || FVector::Dist(Target->GetActorLocation(), GetActorLocation()) > AttackRadius) {
        Roam();
    } else if (IsFacingPlayer()) {
        Chase();
    }
}

bool AEnemy::IsFacingPlayer()
{
    const float FovDegrees = 45;
    const float CosFov = FMath::Cos(FMath::DegreesToRadians(FovDegrees));

    FVector PlayerDirection = Target->GetActorLocation() - GetActorLocation();
    PlayerDirection.Normalize();

    float Dot = FVector::DotProduct(GetActorForwardVector(), PlayerDirection);
    return Dot >= CosFov;
}

bool AEnemy::CollisionCheck()
{
    float Distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
    UE_LOG(LogTemp, Warning, TEXT("%f"), Distance);
    return Distance < CollidingTolerance;
}

void AEnemy::Roam()
{
    auto AIController = Cast<AAIController>(GetController());

    if (AIController && !IsRoaming) {
        float X = (FMath::Rand() % (RoamRadius + 1)) - RoamRadius / 2;
        float Y = (FMath::Rand() % (RoamRadius + 1)) - RoamRadius / 2;
        FVector NewPoint = HomeLocation + FVector(X, Y, 0);

        UE_LOG(LogTemp, Warning, TEXT("%f %f %f"), NewPoint.X, NewPoint.Y, NewPoint.Z);

        FAIMoveRequest AIMoveRequest;
        AIMoveRequest.SetGoalLocation(NewPoint);
        AIController->MoveTo(AIMoveRequest);
        IsRoaming = true;
    }
}

void AEnemy::Chase()
{
    auto AIController = Cast<AAIController>(GetController());

    if (AIController) {
        FAIMoveRequest AIMoveRequest;
        AIMoveRequest.SetGoalLocation(Target->GetActorLocation());
        AIController->MoveTo(AIMoveRequest);
    }
}