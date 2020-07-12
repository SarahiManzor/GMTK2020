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

	UPROPERTY(EditAnywhere)
	float DeliveryRange;

	UPROPERTY(EditAnywhere)
	float TopSpeed;

	// Components
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GuideMesh;

	// References

	// State
	FVector DeliveryLocation;

	float Health;

	int32 TotalDeliveries;

	float ReverseTime;

	// ----------Custom Functions----------
public:
	void SetDeliveryLocation(FVector NewLocation);
	float GetSpeed();
	float GetDeliveryRange();
	void Reverse(float WorldRange);
	void ThrowDelivery();

	UFUNCTION(BlueprintCallable)
	int32 GetTotalDeliveries() {return TotalDeliveries;}

	UFUNCTION(BlueprintCallable)
	float GetHealth() {return Health;}

protected:
private:
	void AddForwardForce();
	void AddTurnForce(float AxisValue);
	void UpdateGuideMarker();

};