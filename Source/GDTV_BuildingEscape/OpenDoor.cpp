// Copyright Precious Navarro 2020

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//Initializing variables
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw += InitialYaw;
    OpenAngle = InitialYaw;

    //Getting Actor for Pressure Plate to check
    ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//triggers opening the door when Actor touches the Pressure Plate
	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpensDoor))
	{
	    OpenDoor(DeltaTime);
	    DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
    else
    {
        if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
        {
            CloseDoor(DeltaTime);
        }
    }

}

/**
 * Function to rotate the door open.
 * @param DeltaTime
 */
void UOpenDoor::OpenDoor(float DeltaTime)
{
    OpenAngle = FMath::Lerp(OpenAngle, TargetYaw, DeltaTime * 0.5f);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = OpenAngle;
    GetOwner()->SetActorRotation(DoorRotation);
}

/**
 * Function to close door.
 * @param DeltaTime
 */
void UOpenDoor::CloseDoor(float DeltaTime)
{
    OpenAngle = FMath::Lerp(OpenAngle, InitialYaw, DeltaTime * 1.0f);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = OpenAngle;
    GetOwner()->SetActorRotation(DoorRotation);
}