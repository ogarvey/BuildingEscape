// Copyright OGDS 2018.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Owner = GetOwner();
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void UOpenDoor::OpenDoor()
{

	FRotator Rotator = FRotator(0.0f, OpenAngle, 0.0f);

	Owner->SetActorRotation(Rotator);
}

void UOpenDoor::CloseDoor()
{

	FRotator Rotator = FRotator(0.0f, -OpenAngle, 0.0f);

	Owner->SetActorRotation(Rotator);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (DoorLastOpenTime > 0.f && DoorLastOpenTime < CurrentTime - DoorCloseDelay)
		UE_LOG(LogTemp, Warning, TEXT("Closing Door..."))//CloseDoor();
}

