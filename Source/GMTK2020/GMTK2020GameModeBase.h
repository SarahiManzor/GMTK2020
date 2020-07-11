// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMTK2020GameModeBase.generated.h"

class ADeliveryCar;
class ADeliveryTarget;
class UWorldChunker;

UCLASS()
class GMTK2020_API AGMTK2020GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	AGMTK2020GameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// ----------Variables----------
public:
protected:
private:
	// Config

	// Components
	UPROPERTY(VisibleAnywhere)
	UWorldChunker* WorldChunker;

	// References
	ADeliveryCar* MainCar;
	TArray<ADeliveryTarget*> DeliveryLocation;

	// State

// ----------Custom Functions----------
public:
protected:

	
};
