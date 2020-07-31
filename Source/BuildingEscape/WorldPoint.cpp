// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldPoint.h"

// Sets default values for this component's properties
UWorldPoint::UWorldPoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWorldPoint::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWorldPoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

