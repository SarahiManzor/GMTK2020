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
}

// Called when the game starts or when spawned
void ADeliveryCar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADeliveryCar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddForwardForce();
	UpdateGuideMarker();

	// Todo Handle outside of this class
	if ((Mesh->GetComponentLocation() - DeliveryLocation).Size() < DeliveryRange)
	{
		ThrowDelivery();
	}
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
	DeliveryLocation = FVector(FMath::FRandRange(-10000.0f, 10000.0f), FMath::FRandRange(-10000.0f, 10000.0f), 0.0f);
}

void ADeliveryCar::UpdateGuideMarker()
{
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GuideMesh->GetComponentLocation(), DeliveryLocation);
	LookAtRotator.Pitch = 0.0f;

	GuideMesh->SetWorldRotation(LookAtRotator);
}

