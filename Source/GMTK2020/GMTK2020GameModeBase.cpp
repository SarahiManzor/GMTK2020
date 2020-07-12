// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "GMTK2020GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DeliveryCar.h"
#include "DeliveryTarget.h"
#include "WorldChunker.h"

AGMTK2020GameModeBase::AGMTK2020GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WorldChunker = CreateDefaultSubobject<UWorldChunker>(TEXT("LevelChunker"));

	MaximumNextRange = 100000.0f;
}

void AGMTK2020GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		MainCar = Cast<ADeliveryCar>(PC->GetPawn());
	}

	if (WorldChunker)
	{
		WorldChunker->Update(MainCar->GetActorLocation());
	}

	SpawnRandomDeliveryLocations();
	SetNewTarget();
}

void AGMTK2020GameModeBase::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	//UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), MainCar->GetSpeed());

	CheckDelivery();
	CheckCarBounds();
	
	//if (WorldChunker)
	//{
	//	WorldChunker->Update(MainCar->GetActorLocation());
	//}
}

void AGMTK2020GameModeBase::SpawnRandomDeliveryLocations()
{
	if (!DeliveryLocationClass) return;

	float WorldRange = WorldChunker != nullptr ? WorldChunker->GetWorldBoundDistance() : 100000.0f; 
	for (int32 i = 0; i < TotalLocations; i++)
	{
		float RandomX = FMath::FRandRange(-WorldRange, WorldRange);
		float RandomY = FMath::FRandRange(-WorldRange, WorldRange);

		if (FVector2D(RandomX, RandomY).Size() < 1000.0f)
		{
			i--;
			continue;
		}

		ADeliveryTarget* Target = GetWorld()->SpawnActor<ADeliveryTarget>(DeliveryLocationClass, FVector(RandomX, RandomY, 20.0f), FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f));
		DeliveryLocations.Add(Target);
	}
}

void AGMTK2020GameModeBase::SetNewTarget()
{
	if (!MainCar) return;

	while (true)
	{
		int32 RandomIndex = FMath::RandRange(0, DeliveryLocations.Num() - 1);
		ADeliveryTarget* NewTarget = DeliveryLocations[RandomIndex];

		if (CurrentTarget == nullptr && FVector::Distance(FVector::ZeroVector, NewTarget->GetActorLocation()) < MaximumNextRange 
		|| CurrentTarget != nullptr && NewTarget != CurrentTarget && CurrentTarget->GetDistanceTo(NewTarget) < MaximumNextRange)
		{
			CurrentTarget = NewTarget;
			MainCar->SetDeliveryLocation(NewTarget->GetActorLocation());
			break;
		}
	}
}

void AGMTK2020GameModeBase::CheckDelivery()
{
	if (!MainCar || !CurrentTarget) return;

	if (CurrentTarget->IsDeliverable(MainCar->GetActorLocation(), MainCar->GetDeliveryRange()))
	{
		MainCar->ThrowDelivery();
		SetNewTarget();
	}
}

void AGMTK2020GameModeBase::CheckCarBounds()
{
	float WorldRange = WorldChunker != nullptr ? WorldChunker->GetWorldBoundDistance() : 100000.0f;

	if (FMath::Abs(MainCar->GetActorLocation().X) > WorldRange || FMath::Abs(MainCar->GetActorLocation().Y) > WorldRange)
	{
		MainCar->Reverse(WorldRange);
	}
}
