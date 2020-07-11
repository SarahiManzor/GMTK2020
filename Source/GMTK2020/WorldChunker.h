// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WorldChunker.generated.h"

class ADeliveryTarget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GMTK2020_API UWorldChunker : public UActorComponent
{
	GENERATED_BODY()

// ----------Base Functions----------
public:
	UWorldChunker();

protected:
	virtual void BeginPlay() override;

// ----------Variables----------
public:
protected:
private:
	// Config
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADeliveryTarget> DeliveryTargetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Cactus;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Ground;

	UPROPERTY(EditAnywhere)
	int32 ChunkWidth;

	UPROPERTY(EditAnywhere)
	int32 ChunkingRange;

	UPROPERTY(EditAnywhere)
	int32 CactusInChunk;

	// Components

	// References

	// State
	TMap<TTuple<int32, int32>, bool> Chunks;

	FVector2D LastChunk;

// ----------Custom Functions----------
public:
	void Update(FVector PlayerLocation);
protected:
private:
		
};
