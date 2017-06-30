// Copyright yotta 2017

#include "TheBeginning.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UGrabber::SetPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Component PhysicsHandle."), *GetOwner()->GetName());
	}
}

void UGrabber::SetInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Component Input."), *GetOwner()->GetName());
	}
}

void UGrabber::SetPlayerController()
{
	Owner = Cast<APawn>(GetOwner()); // get player controller bottom-up
	if (Owner)
	{
		PlayerController = Cast<APlayerController>(Owner->GetController());
	}
	if (!Owner || !PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController missing."));
	}

}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed."));

	FHitResult Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			Hit.GetActor()->GetActorLocation(),
			Hit.GetActor()->GetActorRotation()
		);
		UE_LOG(LogTemp, Warning, TEXT("ComponentToGrab name is %s."), *ComponentToGrab->GetName());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released."));

	PhysicsHandle->ReleaseComponent();
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SetPlayerController();
	SetPhysicsHandle();
	SetInput();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	))
	{
		UE_LOG(LogTemp, Warning, TEXT("Is Hitting %s"), *(Hit.GetActor()->GetName()));
	}
	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineStart()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	PlayerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	return ViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	PlayerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	return ViewPointLocation + ViewPointRotation.Vector() * Reach;//= ViewPointLocation + PlayerController->GetActorForwardVector() * DebugLineLength;
}
