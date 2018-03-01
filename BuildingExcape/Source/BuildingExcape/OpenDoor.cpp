// Copyright Adit Suvarna 2017

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "math.h"


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

	// ...
	Owner = GetOwner();

	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("Could not find owner"));
	}

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure plate uninitialized."));
	}
}

void UOpenDoor::OpenDoor() {
	if (!Owner) { return;  }
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor() {
	if (!Owner) { return; }
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll Trigger Volume
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > 27.5f) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return 0; }
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (const auto* Actor : OverlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("%s is on plate"), *Actor->GetName());
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	FString TheFloatStr = FString::SanitizeFloat(TotalMass);
	UE_LOG(LogTemp, Warning, TEXT("Mass on Plate: %s"), *TheFloatStr);
	return TotalMass;
}

