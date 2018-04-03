// Copyright OGDS 2018.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

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

}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	if (!PressurePlate) { return 0.f; }
	float TotalMass = 0.f;
	TArray<AActor*> ActorsOnPlate;
	//UPrimitiveComponent* Comp;
	PressurePlate->GetOverlappingActors(OUT ActorsOnPlate);

	UE_LOG(LogTemp, Warning, TEXT("Actors on plate: %d"), ActorsOnPlate.Num());

	if (ActorsOnPlate.Num() > 0)
	{
		for (auto& Actor : ActorsOnPlate)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return TotalMass;
}

