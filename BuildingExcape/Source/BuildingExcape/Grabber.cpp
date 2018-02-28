// Copyright Adit Suvarna 2017

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


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

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s has no component of type 'Physics Handler'"), *GetOwner()->GetName());
	}

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component found"));
		/// Bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s has no component of type 'InputComponent'"), *GetOwner()->GetName());
	}

}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbing"));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Releasing"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		PlayerViewPointLocation,
		PlayerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Viewpoint at %s, rotated at %s"),
	//	*PlayerViewPointLocation.ToString(), 
	//	*PlayerViewPointRotation.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// Draw a red trace in the world
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor::Red,
		false,
		0.f,
		0.f,
		2.f
	);

	// Ray-cast out to reach distance
	FHitResult Hit;

	// Setup query params
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());


	bool IsHit = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// See what we hit
	if (IsHit) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(Hit.GetActor()->GetName()));
	}
}

