// Copyright Precious Navarro 2020

#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
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

	Reach = 100.f;

    //Check for Physics Handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle)
    {
        
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component included in %s"), *GetOwner()->GetName());
    }

    //BindAction
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputComponent found on %s"), *GetOwner()->GetName());
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab working!"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Get Player Viewpoint
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
            OUT PlayerViewpointLocation,
            OUT PlayerViewpointRotation
    );

    //Draw a line from player showing reach
    FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
    LineTraceEnd.Rotation() = PlayerViewpointRotation;
    
    DrawDebugLine(
            GetWorld(),
            PlayerViewpointLocation,
            LineTraceEnd,
            FColor(0, 0, 255),
            false,
            0.f,
            0,
            5.f
    );
    FHitResult Hit;
	//Ray cast out to a certain distance (Reach)

    //Logging out to test
    FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewpointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams
    );
	//See what it hits
    AActor* ActorHit = Hit.GetActor();
    if(ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s was hit!"), *(ActorHit)->GetName());
    }
}

