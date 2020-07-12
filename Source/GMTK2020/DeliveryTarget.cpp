// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryTarget.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADeliveryTarget::ADeliveryTarget()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ADeliveryTarget::BeginPlay()
{
	Super::BeginPlay();
	
	DeliveryLocation = GetActorForwardVector() * 600.0f + GetActorLocation();
}

// Called every frame
void ADeliveryTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADeliveryTarget::IsDeliverable(FVector CarPosition, float DeliveryRange)
{	
	return FVector::Distance(DeliveryLocation, CarPosition) < DeliveryRange && FVector::DotProduct(GetActorForwardVector(), (CarPosition - DeliveryLocation)) > 0.0f;
}

