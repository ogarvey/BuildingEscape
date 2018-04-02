// Copyright OGDS 2018.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

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
	Controller = GetWorld()->GetFirstPlayerController();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		FString ActorName = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("%s missing a physics handle component"), *ActorName);
	}
}

void UGrabber::SetupInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing an input component"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return Hit;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor())
	{
		FString Name = Hit.GetActor()->GetName();

		UE_LOG(LogTemp, Warning, TEXT("Grabbing %s"), *Name);

		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation()
		);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

FVector UGrabber::GetReachLineStart()
{
	FVector Location;
	FRotator Rotator;

	Controller->GetPlayerViewPoint(OUT Location, OUT Rotator);

	FVector LineTraceDirection = Rotator.Vector();
	return Location;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector Location;
	FRotator Rotator;

	Controller->GetPlayerViewPoint(OUT Location, OUT Rotator);

	FVector LineTraceDirection = Rotator.Vector();
	return Location + (LineTraceDirection * GrabReach);
}