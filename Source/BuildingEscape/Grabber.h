// Copyright OGDS 2018.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
// Generated must be last include
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	APlayerController* Controller;
	
	UPROPERTY(EditAnywhere)
		float GrabReach = 80.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* Input = nullptr;

	void Grab();

	void Release();

	FVector GetReachLineStart();

	void FindPhysicsHandleComponent();

	void SetupInputComponent();

	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineEnd();
};
