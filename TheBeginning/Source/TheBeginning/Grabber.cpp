// Copyright yotta 2017

#include "TheBeginning.h"
#include "Grabber.h"


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

	// ...
	Owner = Cast<APawn>(GetOwner()); // get player controller bottom-up
	if (Owner)
	{
		PlayerController = Cast<APlayerController>(Owner->GetController());
		if (PlayerController)
			CastDone = true;
	}

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Component PhysicsHandle."), *GetOwner()->GetName());
	}

	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Component Input."), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CastDone)
	{
		FVector ViewPointLocation;
		FRotator ViewPointRotation;
		PlayerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
		//UE_LOG(LogTemp, Warning, TEXT("Location is %s, Rotator is %s"), *Location.ToString(), *Rotator.ToString());

		//FVector LineTraceEnd = ViewPointLocation + PlayerController->GetActorForwardVector() * DebugLineLength;
		FVector LineTraceEnd = ViewPointLocation + ViewPointRotation.Vector() * Reach;

		DrawDebugLine(
			GetWorld(),
			ViewPointLocation,
			LineTraceEnd,
			FColor::Red,
			false,
			0.f,
			0.f,
			10.f
		);
		
		//UE_LOG(LogTemp, Warning, TEXT("begin hit"));

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByObjectType(
			Hit,
			ViewPointLocation,
			LineTraceEnd,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
			FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
		))
		{
			UE_LOG(LogTemp, Warning, TEXT("is Hitting %s"), *(Hit.GetActor()->GetName()));
		}



	}
	// ...
}

