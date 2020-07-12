// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeliveryTarget.generated.h"

class UStaticMeshComponent;
class AGMTK2020GameModeBase;

UCLASS()
class GMTK2020_API ADeliveryTarget : public AActor
{
	GENERATED_BODY()
	
// ----------Base Functions----------
public:
	ADeliveryTarget();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);

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
	AGMTK2020GameModeBase* GameMode;

	// State

// ----------Custom Functions----------
public:
	bool IsDeliverable(FVector CarPosition, float DeliveryRange);
	FVector GetDeliveryTargetLocation();
protected:
private:
};
