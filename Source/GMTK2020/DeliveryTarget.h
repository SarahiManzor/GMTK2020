// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeliveryTarget.generated.h"

class UStaticMeshComponent;

UCLASS()
class GMTK2020_API ADeliveryTarget : public AActor
{
	GENERATED_BODY()
	
// ----------Base Functions----------
public:
	ADeliveryTarget();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// ----------Variables----------
public:
protected:
private:
	// Config
	FVector DeliveryLocation;

	// Components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// References

	// State

// ----------Custom Functions----------
public:
	bool IsDeliverable(FVector CarPosition, float DeliveryRange);
protected:
private:
};
