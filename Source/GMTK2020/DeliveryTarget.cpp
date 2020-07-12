// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliveryTarget.h"
#include "Components/StaticMeshComponent.h"
#include "GMTK2020GameModeBase.h"

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
	
	DeliveryLocation = GetActorForwardVector() * 425.0f + GetActorLocation();

	Mesh->OnComponentHit.AddDynamic(this, &ADeliveryTarget::OnHit);

	GameMode = Cast<AGMTK2020GameModeBase>(GetWorld()->GetAuthGameMode());
}

void ADeliveryTarget::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName().Contains("pizza") && IsDeliverable(OtherActor->GetActorLocation(), 1000.0f))
	{
		GameMode->CheckDelivery(this);
	}
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

