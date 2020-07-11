// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DeliveryCar.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;

UCLASS()
class GMTK2020_API ADeliveryCar : public APawn
{
	GENERATED_BODY()

// ----------Base Functions----------
public:
	ADeliveryCar();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ----------Variables----------
public:
protected:
private:
	// Config
	UPROPERTY(EditAnywhere)
	float AccelerationForce; // Force applied for moving forward

	UPROPERTY(EditAnywhere)
	float TurnForce; // Force applied for turning

	// Components
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// References

	// State

	// ----------Custom Functions----------
public:
protected:
private:
	void AddForwardForce();
	void AddTurnForce(float AxisValue);
	void ThrowDelivery();

};
