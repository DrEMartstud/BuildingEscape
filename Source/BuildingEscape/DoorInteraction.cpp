// Coded by DrEM! All rights belong to him!


#include "DoorInteraction.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UDoorInteraction::UDoorInteraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorInteraction::BeginPlay()
{
	Super::BeginPlay();
	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	openingAngle += initialYaw;
	if(!pressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has no pressure plate set."), *GetOwner()->GetName());
	}
	// doorYawRotation.Yaw = 90.f;
	ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UDoorInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(pressurePlate && pressurePlate->IsOverlappingActor(ActorThatOpensDoor)){
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
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
}
void UDoorInteraction::CloseDoor(float deltaTime) {
	FRotator doorRotation = GetOwner()->GetActorRotation();
	currentYaw = FMath::FInterpTo(currentYaw, initialYaw, deltaTime, doorClosingSpeed);
	doorRotation.Yaw = currentYaw;
	GetOwner()->SetActorRotation(doorRotation);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
}