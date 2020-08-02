// Coded by DrEM! All rights belong to him!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"

#include "DoorInteraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDoorInteraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorInteraction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float deltaTime);
	void CloseDoor(float deltaTime);

	float TotalMassOfActorsOnPlate() const;
	void FindAudioComponent();
	void FindPressurePlate() const;

	//Used to check, what state the door is to play sound
	bool doorIsClosed = true;
private:
	float initialYaw;
	float currentYaw;
	UPROPERTY(EditAnywhere)
	float openingAngle = 90.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
	float doorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float doorClosingDelay = 2.f;
	UPROPERTY(EditAnywhere)
	float doorOpeningSpeed = 1.f;
	UPROPERTY(EditAnywhere)
	float doorClosingSpeed = 2.f;
	
	UPROPERTY(EditAnywhere)
	float massToOpenDoors = 50.f;

	UPROPERTY()
	UAudioComponent* audioComponent = nullptr;
};
