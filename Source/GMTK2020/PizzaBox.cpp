// Fill out your copyright notice in the Description page of Project Settings.


#include "PizzaBox.h"

// Sets default values
APizzaBox::APizzaBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void APizzaBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APizzaBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UStaticMeshComponent* APizzaBox::GetMesh()
{
	return Mesh;
}

