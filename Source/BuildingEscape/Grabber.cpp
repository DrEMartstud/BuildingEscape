// Coded by DrEM! All rights belong to him!

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();
	FindPhysicsHande();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

//Moves held object
	if (physicsHandle->GrabbedComponent) {
		if (!physicsHandle) {return;}
		physicsHandle->SetTargetLocation(GetPlayersReach());
	}
}






void UGrabber::SetupInputComponent() {
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent) {
		// UE_LOG(LogTemp, Warning, TEXT("Found input component for %s"), *GetOwner()->GetName());
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHande() {
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle){ 
		UE_LOG(LogTemp, Error, TEXT("No physicsHande component was found on %s!"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {

	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
	AActor* actorHit = hitResult.GetActor();
	if (actorHit) { if (!physicsHandle) {return;} 
	physicsHandle->GrabComponentAtLocation(
		componentToGrab,
		NAME_None,
		GetPlayersReach()
	);
	}
}

void UGrabber::Release() {
	if (!physicsHandle) {return;}
	physicsHandle->ReleaseComponent();
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	//Ray-casting to a distance
	FHitResult hit;

	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetPlayersWorldPosition(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	return hit;
}


FVector UGrabber::GetPlayersReach() const {
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}

FVector UGrabber::GetPlayersWorldPosition() const {
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	return playerViewPointLocation;
}