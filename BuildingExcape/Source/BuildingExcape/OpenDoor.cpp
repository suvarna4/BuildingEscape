// Copyright Adit Suvarna 2017

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void UOpenDoor::OpenDoor() {
	AActor* Owner = GetOwner();

	float OldYaw = Owner->GetActorRotation().Yaw;

	if (floor(OldYaw + 0.5f) != 90) {
		OldYaw += 1.f;
	}

	FRotator NewRotation = FRotator(0.0f, OldYaw, 0.0f);

	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor() {
	AActor* Owner = GetOwner();

	float OldYaw = Owner->GetActorRotation().Yaw;

	if (floor(OldYaw + 0.5f) != 0) {
		OldYaw -= 1.f;
	}

	FRotator NewRotation = FRotator(0.0f, OldYaw, 0.0f);

	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll Trigger Volume
	// If the ActorThatOpens is in the volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}

