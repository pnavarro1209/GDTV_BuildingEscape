// Copyright Precious Navarro 2020

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

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

	FindPhysicsHandle();

    SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//if physics handle is attached
	if(!PhysicsHandle) {return;}
	if(PhysicsHandle->GrabbedComponent)
	{
		UpdatePlayerViewpoint();
		PhysicsHandle->SetTargetLocation(GetReachLocation());
	}
	
}

void UGrabber::Grab()
{
	UpdatePlayerViewpoint();
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	if(ActorHit)
	{
		if(!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation
					(
			            ComponentToGrab,
			            NAME_None,
			            GetReachLocation()
			        );
	}

	
}

void UGrabber::Release()
{
	if(!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{
        UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component included in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector LineTraceEnd = GetReachLocation();
	LineTraceEnd.Rotation() = PlayerViewpointRotation;
	FHitResult Hit;
	
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewpointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParams 
    );
	return Hit;
}

FVector UGrabber::GetReachLocation() const
{
	//Ray cast out to a certain distance (Reach)
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	return LineTraceEnd;
}

void UGrabber::UpdatePlayerViewpoint()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
            OUT PlayerViewpointLocation,
            OUT PlayerViewpointRotation
    );
}