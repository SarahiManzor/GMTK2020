// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldChunker.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UWorldChunker::UWorldChunker()
{
	PrimaryComponentTick.bCanEverTick = false;

	ChunkWidth = 1000;
	ChunkingRange = 3;
	CactusInChunk = 50;

	LastChunk = FVector2D(-1000.0f, -1000.0f);
}


// Called when the game starts
void UWorldChunker::BeginPlay()
{
	Super::BeginPlay();
	
}

void UWorldChunker::Update(FVector PlayerLocation)
{
	UE_LOG(LogTemp, Warning, TEXT("Update"));

	FVector2D PlayerChunk = FVector2D((int32) (PlayerLocation.X / ChunkWidth), (int32)(PlayerLocation.Y / ChunkWidth));

	if (LastChunk == PlayerChunk) return;

	LastChunk = PlayerChunk;

	for (int32 x = PlayerChunk.X - ChunkingRange; x < PlayerChunk.X + ChunkingRange; x++)
	{
		for (int32 y = PlayerChunk.X - ChunkingRange; y < PlayerChunk.Y + ChunkingRange; y++)
		{
			if (Chunks.Contains(MakeTuple(x, y)) && Chunks[MakeTuple(x, y)]) continue;
			Chunks.Add(MakeTuple(x, y), true);

			if (Ground)
			{
				FVector Pos = FVector(x * ChunkWidth + ChunkWidth / 2.0f, y * ChunkWidth + ChunkWidth / 2.0f, 0.0f);
				AActor* SpawnedGround = GetWorld()->SpawnActor<AActor>(Ground, Pos, FRotator::ZeroRotator);
				SpawnedGround->SetActorScale3D(FVector(ChunkWidth / 1000.0f, ChunkWidth / 1000.0f, 1.0f));

			}

			if (Cactus)
			{
				for (int32 i = 0; i < CactusInChunk; i++)
				{
					FVector RandomPos = FVector(FMath::FRandRange(0, ChunkWidth) + x * ChunkWidth, FMath::FRandRange(0, ChunkWidth) + y * ChunkWidth, 20.0f);
					GetWorld()->SpawnActor<AActor>(Cactus, RandomPos, FRotator(0.0f, FMath::FRandRange(0.0f, 360.0f), 0.0f));
				}
			}
		}
	}
}

