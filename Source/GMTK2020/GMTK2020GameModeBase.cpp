// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "GMTK2020GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DeliveryCar.h"
#include "WorldChunker.h"

AGMTK2020GameModeBase::AGMTK2020GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	WorldChunker = CreateDefaultSubobject<UWorldChunker>(TEXT("LevelChunker"));
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
}

void AGMTK2020GameModeBase::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Ticking"));
	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), MainCar->GetSpeed());
	
	//if (WorldChunker)
	//{
	//	WorldChunker->Update(MainCar->GetActorLocation());
	//}
}
