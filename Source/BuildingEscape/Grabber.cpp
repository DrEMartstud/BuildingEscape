// Coded by DrEM! All rights belong to him!

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHande();
	SetupInputComponent();
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
	if (physicsHandle){ 
		//physicsHandle is found
	} else {
		UE_LOG(LogTemp, Error, TEXT("ERROR! No physicsHande component was found on %s!"), *GetOwner()->GetName());
	}
}
void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbing!"));
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
	if (hitResult.GetActor()) {
	physicsHandle->GrabComponentAtLocation(
		componentToGrab,
		NAME_None,
		lineTraceEnd
	);
	}
}
void UGrabber::Release(){
	UE_LOG(LogTemp, Warning, TEXT("Releasing!"));
	physicsHandle->ReleaseComponent();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s \nRotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());
	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
		//move held object

	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s \nRotation: %s"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());
	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	//Draw line from player to calculate reach
	DrawDebugLine(
		GetWorld(),
		playerViewPointLocation,
		lineTraceEnd,
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f
	);
	//Ray-casting to a distance
	FHitResult hit;

	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		playerViewPointLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	AActor* hitActor = hit.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Line hits: %s"), *(hitActor->GetName()));
	}
	return hit;
}