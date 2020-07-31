// Coded by DrEM! All rights belong to him!

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"
#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
FVector playerViewPointLocation;
FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s \nRotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());
	//Draw line from player to calculate reach
	FVector sightLineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	DrawDebugLine(
		GetWorld(),
		playerViewPointLocation,
		sightLineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);
	//Ra-casting to a distance
	FHitResult hit;
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		playerViewPointLocation,
		sightLineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	AActor* hitActor = hit.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Error, TEXT("Line hits: %s"), *(hitActor->GetName()));
	}
}

