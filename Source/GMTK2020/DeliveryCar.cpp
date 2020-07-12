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

// Sets default values
ADeliveryCar::ADeliveryCar()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	// Camera Boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(Mesh);

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Guide Mesh
	GuideMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Guide Mesh"));
	GuideMesh->SetupAttachment(Mesh);

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
}

// Called when the game starts or when spawned
void ADeliveryCar::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ADeliveryCar::OnHit);
}

void ADeliveryCar::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName().Contains("floor")) return;
	
	float Time = UGameplayStatics::GetTimeSeconds(this);
	if (Time - TimeOfHit > InvicibilityTime)
	{
		float Damage = NormalImpulse.Size();
		UE_LOG(LogTemp, Warning, TEXT("Crash Impulse: %f"), Damage);

		if (Damage > 100.0f)
		{
			Health -= Damage;
			UE_LOG(LogTemp, Warning, TEXT("Remaining Health: %f"), Health);
		}
	}
}

// Called every frame
void ADeliveryCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddForwardForce();
	UpdateGuideMarker();
}

// Called to bind functionality to input
void ADeliveryCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("LeftMouse"), IE_Pressed, this, &ADeliveryCar::ThrowDelivery);

	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ADeliveryCar::AddTurnForce);

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
	if (Time - ReverseTime < 1.0f) return;

	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw -= 180.0f;
	SetActorRotation(Rotation);

	FVector ActorLocation = GetActorLocation();
	ActorLocation.X = FMath::Clamp(ActorLocation.X, -WorldRange, WorldRange);
	ActorLocation.Y = FMath::Clamp(ActorLocation.Y, -WorldRange, WorldRange);

	ReverseTime = Time;
}

void ADeliveryCar::AddForwardForce()
{
	//AddMovementInput(GetActorForwardVector(), AccelerationForce);
	if (GetSpeed() < TopSpeed)
	Mesh->AddForce(AccelerationForce * GetActorForwardVector());
	//UE_LOG(LogTemp, Warning, TEXT("Adding Force"));
}

void ADeliveryCar::AddTurnForce(float AxisValue)
{
	Mesh->SetWorldRotation(Mesh->GetComponentRotation() + FRotator(0.0f, AxisValue * TurnForce, 0.0f));
}

void ADeliveryCar::ThrowDelivery()
{
	UE_LOG (LogTemp, Warning, TEXT("Pizza Delivered!"));
	TotalDeliveries += 1;

	// Todo spawn pizza box mesh and throw at house (Make sure car doesnt collide with the box
}

void ADeliveryCar::UpdateGuideMarker()
{
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GuideMesh->GetComponentLocation(), DeliveryLocation);
	LookAtRotator.Pitch = 0.0f;

	GuideMesh->SetWorldRotation(LookAtRotator);
}

