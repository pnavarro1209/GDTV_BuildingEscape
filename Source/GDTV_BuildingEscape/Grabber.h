// Copyright Precious Navarro 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDTV_BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Grab();

	void Release();

	void FindPhysicsHandle();

	void SetupInputComponent();

	//Return first actor in reach with a physics body
	FHitResult GetFirstPhysicsBodyInReach() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    float Reach = 100.f;

    UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;
};
