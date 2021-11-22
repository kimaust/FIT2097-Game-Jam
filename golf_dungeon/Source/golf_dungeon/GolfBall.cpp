// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBall.h"

// Sets default values
AGolfBall::AGolfBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	RootComponent = VisibleComponent;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	VisibleComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetupAttachment(VisibleComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Initialize fields.
	IsCharging = false;
	CanLaunch = false;
	LaunchSpeed = 0;
	CharingSpeed = 2000;
	MaxLaunchSpeed = 5000;
	SnapTolerance = 90;
	LinearDamping = 0.65;
	AngularDamping = 0.65;
	SpringArmLength = 300;
	CameraOffset = FVector(0, 0, 70);
	ProjectilePathInformation.ProjectileRadius = 2;
	ProjectilePathInformation.SimulationFrequency = 25;

	// Set up the physics, camera and possession.
	VisibleComponent->SetSimulatePhysics(true);
	SpringArmComponent->bUsePawnControlRotation = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Game/StarterContent/Props/MaterialSphere.MaterialSphere")
	);
	if (SphereMesh.Succeeded())
		VisibleComponent->SetStaticMesh(SphereMesh.Object);
}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	VisibleComponent->SetLinearDamping(LinearDamping);
	VisibleComponent->SetAngularDamping(AngularDamping);
	SpringArmComponent->TargetArmLength = SpringArmLength;
	GameMode = Cast<Agolf_dungeonGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->SetMaxLaunchSpeed(MaxLaunchSpeed);
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make camera look down towards the ball.
	SpringArmComponent->SetWorldLocation(VisibleComponent->GetComponentLocation() + CameraOffset);

	// Determine whether the ball can be launched.
	FVector CurrentVelocity = VisibleComponent->GetComponentVelocity();
	// CanLaunch = OnGround() && CurrentVelocity.Size() <= SnapTolerance;
	float CurrentSpeed = FMath::Sqrt(FMath::Pow(CurrentVelocity.X, 2) + FMath::Pow(CurrentVelocity.Y, 2));
	CanLaunch = OnGround() && CurrentSpeed  <= SnapTolerance;

	UE_LOG(LogTemp, Warning, TEXT("CanLaunch: %d"), CanLaunch);
	UE_LOG(LogTemp, Warning, TEXT("Velocity: %f"), CurrentSpeed);

	if (CanLaunch) {
		// Stop the ball by setting its velocity to 0 except Z-axis to allow the ball to fall due to gravity.
		VisibleComponent->SetPhysicsLinearVelocity(FVector(0, 0, CurrentVelocity.Z));
		if (IsCharging) {
			// Update launching speed and draw projectile path.
			LaunchSpeed = FMath::Min(LaunchSpeed + DeltaTime * CharingSpeed, MaxLaunchSpeed);
			GameMode->SetLaunchSpeed(LaunchSpeed);
			DrawProjectilePath();
		}
	}
}

// Called to bind functionality to input
void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AGolfBall::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AGolfBall::LookUp);
	PlayerInputComponent->BindAxis("Wheel", this, &AGolfBall::Zoom);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGolfBall::Charge);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AGolfBall::Discharge);
}

void AGolfBall::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AGolfBall::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AGolfBall::Zoom(float AxisValue)
{
	const float ZoomIncrement = 20;
	SpringArmComponent->TargetArmLength += AxisValue * ZoomIncrement * -1;
}

void AGolfBall::Charge()
{
	if (CanLaunch)
		IsCharging = true;
}

void AGolfBall::Discharge()
{
	if (!IsCharging) return;

	IsCharging = false;		
	if (CanLaunch) {
		VisibleComponent->SetPhysicsLinearVelocity(GetLaunchVector());
		if (JumpSound) {
			UGameplayStatics::PlaySound2D(GetWorld(), JumpSound);
		}
		if (JumpEffect) {
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(), JumpEffect, VisibleComponent->GetComponentLocation(), FRotator::ZeroRotator, true);
		}
		CanLaunch = false;
	}
	LaunchSpeed = 0;
	GameMode->SetLaunchSpeed(LaunchSpeed);
}

bool AGolfBall::OnGround()
{
	const float Tolerance = 50;
	const float TraceOffset = 10;
	const float TraceLength = 300;

	FVector Direction = FVector(0, 0, -1);
	FVector StartTrace = VisibleComponent->GetComponentLocation() + Direction * TraceOffset;
	FVector EndTrace = StartTrace + Direction * TraceLength;

	FHitResult LinetraceResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(
		LinetraceResult, StartTrace, EndTrace, ECC_WorldStatic, CollisionParams);

	if (IsHit) {
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), LinetraceResult.Distance);
	}

	return IsHit && LinetraceResult.Distance <= Tolerance;
}

FVector AGolfBall::GetLaunchVector()
{
	return UKismetMathLibrary::GetForwardVector(GetControlRotation()) * LaunchSpeed;
}

void AGolfBall::DrawProjectilePath()
{
	FVector DrawOffset = CameraComponent->GetForwardVector() * 60 + FVector(0, 0, 60);

	FPredictProjectilePathParams PredictParams(
		ProjectilePathInformation.ProjectileRadius, 
		VisibleComponent->GetComponentLocation() + DrawOffset, 
		GetLaunchVector(), 1);
	PredictParams.ActorsToIgnore = TArray<AActor*>{this};
	PredictParams.DrawDebugTime = 1;
	PredictParams.bTraceComplex = true;
	PredictParams.bTraceWithCollision = true;
	PredictParams.TraceChannel = ECollisionChannel::ECC_Visibility;
	PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForOneFrame;
	PredictParams.SimFrequency = ProjectilePathInformation.SimulationFrequency;
	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();

	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(this, PredictParams, Result);
}

bool AGolfBall::IsAttacking()
{
	return !CanLaunch;
}