// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DeliveryCar.generated.h"

class UBoxComponent;
class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class AGMTK2020GameModeBase;
class APizzaBox;
class USoundBase;
class UAudioComponent;

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
	TSubclassOf<APizzaBox> PizzaBox;

	UPROPERTY(EditAnywhere)
	float AccelerationForce; // Force applied for moving forward

	UPROPERTY(EditAnywhere)
	float TurnForce; // Force applied for turning

	UPROPERTY(EditAnywhere)
	float DeliveryForce; // Force applied for turning

	UPROPERTY(EditAnywhere)
	float DeliveryRange;

	UPROPERTY(EditAnywhere)
	float TopSpeed;

	UPROPERTY(EditAnywhere)
	float TotalHealth;

	// Components
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CarMesh;


	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GuideMesh;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftTrye;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* RightTyre;

	UPROPERTY(EditAnywhere)
	USoundBase* DeliverySound;

	UPROPERTY(EditAnywhere)
	USoundBase* CrashSound;

	UPROPERTY(EditAnywhere)
	USoundBase* RocketSound;

	UPROPERTY(EditAnywhere)
	USoundBase* PizzaSound;

	UAudioComponent* PlayingRocketSound;
	
	// References
	AGMTK2020GameModeBase* GameMode;

	// State
	FVector DeliveryLocation;
	FRotator TargetRotation;

	float Health;

	int32 TotalDeliveries;
	int32 PizzasThrown;

	float ReverseTime;

	float TimeOfHit;
	float InvicibilityTime;

	bool bIsPlaying;

	float FrameTime;

	// ----------Custom Functions----------
public:
	void StartEngine();

	void SetDeliveryLocation(FVector NewLocation);
	float GetSpeed();
	float GetDeliveryRange();
	void Reverse(float WorldRange);
	void ThrowDelivery();
	void SuccessfulDelivery();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalDeliveries();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPizzasThrown();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() {return Health;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTotalHealth() { return TotalHealth; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetPlaying() { return bIsPlaying; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndGame();

protected:
private:
	void AddForwardForce();
	void AddTurnForce(float AxisValue);
	void UpdateGuideMarker();
	void UpdateTires();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
