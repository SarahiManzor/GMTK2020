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
	WorldRange *= 0.9f;
	for (int32 i = 0; i < TotalLocations; i++)
	{
		float RandomX = FMath::FRandRange(-WorldRange, WorldRange);
		float RandomY = FMath::FRandRange(-WorldRange, WorldRange);
		FVector SpawnLocation = FVector(RandomX, RandomY, 18.0f);
		
		bool TooClose = false;
		for (int32 j = 0; j < i; j++)
		{
			if (FVector::Distance(SpawnLocation, DeliveryLocations[j]->GetActorLocation()) < 1000.0f)
			{
				TooClose = true;
				break;
			}
		}

		if (SpawnLocation.Size() < 1000.0f || TooClose)
		{
			i--;
			continue;
		}

		ADeliveryTarget* Target = GetWorld()->SpawnActor<ADeliveryTarget>(DeliveryLocationClass, SpawnLocation, FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f));
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
			MainCar->SetDeliveryLocation(NewTarget->GetDeliveryTargetLocation());
			break;
		}
	}
}

bool AGMTK2020GameModeBase::CheckDelivery(ADeliveryTarget* Target)
{
	if (!MainCar || !CurrentTarget || Target != CurrentTarget) return false;

	if (CurrentTarget->IsDeliverable(MainCar->GetActorLocation(), MainCar->GetDeliveryRange()))
	{	
		MainCar->SuccessfulDelivery();
		SetNewTarget();
		return true;
	}

	return false;
}

void AGMTK2020GameModeBase::StartGame()
{
	if (MainCar)
		MainCar->StartEngine();
}

void AGMTK2020GameModeBase::EndGame_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("End Game"));
}

void AGMTK2020GameModeBase::CheckCarBounds()
{
	float WorldRange = WorldChunker != nullptr ? WorldChunker->GetWorldBoundDistance() : 100000.0f;

	if (FMath::Abs(MainCar->GetActorLocation().X) > WorldRange || FMath::Abs(MainCar->GetActorLocation().Y) > WorldRange)
	{
		MainCar->Reverse(WorldRange);
	}
}
