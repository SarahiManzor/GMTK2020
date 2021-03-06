// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryCar.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GMTK2020GameModeBase.h"
#include "PizzaBox.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h" 

// Sets default values
ADeliveryCar::ADeliveryCar()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	SetRootComponent(CarMesh);

	// Camera Boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(CarMesh);

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Guide Mesh
	GuideMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Guide Mesh"));
	GuideMesh->SetupAttachment(CarMesh);

	LeftTrye = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Tyre"));
	LeftTrye->SetupAttachment(CarMesh);

	RightTyre = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Tyre"));
	RightTyre->SetupAttachment(CarMesh);

	// Base speeds
	AccelerationForce = 100.0f;
	TurnForce = 100.0f;

	DeliveryLocation = FVector::ZeroVector;
	DeliveryRange = 200.0f;

	TopSpeed = 15000.0f;

	TotalDeliveries = 0;
	ReverseTime = 0.0f;
	TimeOfHit = 0.0f;
	InvicibilityTime = 0.25f;
	TotalHealth = 100000;
	Health = TotalHealth;
	PizzasThrown = 0;

	bIsPlaying = false;
}

// Called when the game starts or when spawned
void ADeliveryCar::BeginPlay()
{
	Super::BeginPlay();

	CarMesh->OnComponentHit.AddDynamic(this, &ADeliveryCar::OnHit);

	GameMode = Cast<AGMTK2020GameModeBase>(GetWorld()->GetAuthGameMode());

	FTimerHandle Handle;
}

void ADeliveryCar::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName().Contains("floor")) return;
	
	float Time = UGameplayStatics::GetTimeSeconds(this);
	if (Time - TimeOfHit > InvicibilityTime)
	{
		float Damage = NormalImpulse.Size() / 1000.0f;
		UE_LOG(LogTemp, Warning, TEXT("Crash Impulse: %f"), Damage);

		if (Damage > 100.0f)
		{
			Health -= Damage;
			UE_LOG(LogTemp, Warning, TEXT("Remaining Health: %f"), Health);
			TimeOfHit = Time;

			if (Health < 0.0f)
			{
				EndGame_Implementation();
				EndGame();
			}

			if (CrashSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), CrashSound);
			}
		}
	}
}

// Called every frame
void ADeliveryCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddForwardForce();

	FrameTime = DeltaTime;
	UpdateGuideMarker();
	UpdateTires();

	if (GuideMesh->GetVisibleFlag() != bIsPlaying)
		GuideMesh->SetVisibility(bIsPlaying);
}

// Called to bind functionality to input
void ADeliveryCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ThrowPizza"), IE_Pressed, this, &ADeliveryCar::ThrowDelivery);

	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ADeliveryCar::AddTurnForce);

}

void ADeliveryCar::StartEngine()
{
	bIsPlaying = true;

	if (RocketSound)
	{
		PlayingRocketSound = UGameplayStatics::SpawnSound2D(GetWorld(), RocketSound);
	}
}

void ADeliveryCar::SetDeliveryLocation(FVector NewLocation)
{
	DeliveryLocation = NewLocation;
}

float ADeliveryCar::GetSpeed()
{
	return GetVelocity().Size();
}

float ADeliveryCar::GetDeliveryRange()
{
	return DeliveryRange;
}

void ADeliveryCar::Reverse(float WorldRange)
{
	float Time = UGameplayStatics::GetTimeSeconds(this);
	if (Time - ReverseTime < 2.0f) return;

	CarMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	FRotator Rotation = GetActorRotation();

	Rotation.Yaw = UKismetMathLibrary::FindLookAtRotation(GuideMesh->GetComponentLocation(), FVector::ZeroVector).Yaw;
	SetActorRotation(Rotation);


	FVector ActorLocation = GetActorLocation();
	ActorLocation.X = FMath::Clamp(ActorLocation.X, -WorldRange, WorldRange);
	ActorLocation.Y = FMath::Clamp(ActorLocation.Y, -WorldRange, WorldRange);

	ReverseTime = Time;
}

void ADeliveryCar::AddForwardForce()
{
	if (!bIsPlaying) return;

	UE_LOG(LogTemp, Warning, TEXT("Force"));
	//AddMovementInput(GetActorForwardVector(), AccelerationForce);
	if (GetSpeed() < TopSpeed)
	{
		FVector Force = AccelerationForce * GetActorForwardVector() * FrameTime;
		CarMesh->AddImpulse(Force);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Adding Force"));
}

void ADeliveryCar::AddTurnForce(float AxisValue)
{
	if (bIsPlaying)
		CarMesh->SetWorldRotation(CarMesh->GetComponentRotation() + FRotator(0.0f, AxisValue * TurnForce * (FrameTime *  120.0f), 0.0f));
	
	float zRotation = 0.0f;
	if (AxisValue < -0.1)
	{
		zRotation = -40.0f;
	}
	else if (AxisValue > 0.1)
	{
		zRotation = 40.0f;
	}

	FRotator Rotation = FRotator(0.0f, zRotation, 0.0f);
	TargetRotation = Rotation;
}

void ADeliveryCar::UpdateTires()
{
	FRotator CurrentRotation = LeftTrye->GetRelativeRotation();
	FRotator NewRotation = UKismetMathLibrary::RLerp(CurrentRotation, TargetRotation, 0.1f, true);
	LeftTrye->SetRelativeRotation(NewRotation);
	RightTyre->SetRelativeRotation(NewRotation);
}

void ADeliveryCar::ThrowDelivery()
{
	UE_LOG(LogTemp, Warning, TEXT("Pizza Thrown"));

	// Todo spawn pizza box mesh and throw at house (Make sure car doesnt collide with the box
	if (PizzaBox)
	{
		PizzasThrown += 1;
		APizzaBox* Pizza = GetWorld()->SpawnActor<APizzaBox>(PizzaBox, GetActorLocation() + FVector::UpVector * 160.0f, FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f));
		FVector ThrowDirection = DeliveryLocation - GetActorLocation();
		ThrowDirection.Z = 0.0f;
		ThrowDirection.Normalize();

		UE_LOG(LogTemp, Warning, TEXT("Throw velocity: %s"), *GetVelocity().ToCompactString());
		bool InRange = FVector::Distance(DeliveryLocation, GetActorLocation()) < DeliveryRange;
		Pizza->GetMesh()->AddImpulse((ThrowDirection * DeliveryForce * FrameTime + (InRange ? ThrowDirection * DeliveryForce * FrameTime : (GetVelocity() / 1000.0f))));
		
		if (PizzaSound)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), PizzaSound);
		}
	}
}

void ADeliveryCar::SuccessfulDelivery()
{
	UE_LOG(LogTemp, Warning, TEXT("Pizza Delivered!"));
	TotalDeliveries += 1;

	if (DeliverySound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DeliverySound);
	}
}

int32 ADeliveryCar::GetTotalDeliveries()
{
	return TotalDeliveries;
}

int32 ADeliveryCar::GetPizzasThrown()
{
	return PizzasThrown;
}

void ADeliveryCar::EndGame_Implementation()
{
	bIsPlaying = false;
	UE_LOG(LogTemp, Warning, TEXT("Dead"));

	if (GameMode)
	{
		GameMode->EndGame_Implementation();
		GameMode->EndGame();
	}

	if (PlayingRocketSound)
	{
		PlayingRocketSound->SetPaused(true);
	}
}

void ADeliveryCar::UpdateGuideMarker()
{
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GuideMesh->GetComponentLocation(), DeliveryLocation);
	LookAtRotator.Pitch = 0.0f;

	GuideMesh->SetWorldRotation(LookAtRotator);
}

