// Coded by DrEM! All rights belong to him!
#include "DoorInteraction.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#define OUT
// Sets default values for this component's properties
UDoorInteraction::UDoorInteraction() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDoorInteraction::BeginPlay() {
	Super::BeginPlay();
	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	openingAngle += initialYaw;
	FindPressurePlate();
	FindAudioComponent();
	// doorYawRotation.Yaw = 90.f;
	
}

// Called every frame
void UDoorInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActorsOnPlate() >= massToOpenDoors) {
	OpenDoor(DeltaTime);
	doorLastOpened = GetWorld()->GetTimeSeconds();
	} else {
		if(GetWorld()->GetTimeSeconds() - doorLastOpened > doorClosingDelay){
			CloseDoor(DeltaTime);	
		}
	}
}



void UDoorInteraction::OpenDoor(float deltaTime) {
	FRotator doorRotation = GetOwner()->GetActorRotation();
	currentYaw = FMath::FInterpTo(currentYaw, openingAngle, deltaTime, doorOpeningSpeed);
	doorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotation);
	if (!audioComponent) {return;}
	if (doorIsClosed) {
		audioComponent->Play();
		doorIsClosed = false;
	}
}

void UDoorInteraction::CloseDoor(float deltaTime) {
	FRotator doorRotation = GetOwner()->GetActorRotation();
	currentYaw = FMath::FInterpTo(currentYaw, initialYaw, deltaTime, doorClosingSpeed);
	doorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotation);
	if (!audioComponent) {return;}
	if (!doorIsClosed) {
		audioComponent->Play();
		doorIsClosed = true;
	}
}

float UDoorInteraction::TotalMassOfActorsOnPlate() const {
	float totalMass = 0.f;

	//Finds overlapping actors
	TArray<AActor*> overlappingActors;
	if(!pressurePlate) {return totalMass;}
	pressurePlate->GetOverlappingActors(OUT overlappingActors);
	for (AActor* actor : overlappingActors) {
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	//Sums total mass
	return totalMass;
}

void UDoorInteraction::FindAudioComponent() {
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!audioComponent) {UE_LOG(LogTemp, Error, TEXT("%s is missing audio component!"), *GetOwner()->GetName());}
}

void UDoorInteraction::FindPressurePlate() const {
	if(!pressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has no pressure plate set."), *GetOwner()->GetName());
	}
}