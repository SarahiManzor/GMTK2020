// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryCar.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h" 

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

	// Base speeds
	AccelerationForce = 100.0f;
	TurnForce = 100.0f;
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
}

// Called to bind functionality to input
void ADeliveryCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("LeftMouse"), IE_Pressed, this, &ADeliveryCar::ThrowDelivery);

	PlayerInputComponent->BindAxis(TEXT("Right"), this, &ADeliveryCar::AddTurnForce);

}

void ADeliveryCar::AddForwardForce()
{
	//AddMovementInput(GetActorForwardVector(), AccelerationForce);
	Mesh->AddForce(AccelerationForce * GetActorForwardVector());
	//UE_LOG(LogTemp, Warning, TEXT("Adding Force"));
}

void ADeliveryCar::AddTurnForce(float AxisValue)
{
	Mesh->SetWorldRotation(Mesh->GetComponentRotation() + FRotator(0.0f, AxisValue * TurnForce, 0.0f));
}

void ADeliveryCar::ThrowDelivery()
{

}

