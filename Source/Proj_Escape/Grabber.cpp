// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

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

	this->LineLength = 100.0f;
	UE_LOG(LogTemp, Warning, TEXT("Grabber Here~~"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector tmp_PlayerViewPointLocation;
	FRotator tmp_PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT tmp_PlayerViewPointLocation,
		OUT tmp_PlayerViewPointRotator
	);

	/*UE_LOG(LogTemp, Warning, TEXT("PlayerViewPortLocation:%s, Rotator:%s"), 
		*tmp_PlayerViewPointLocation.ToString(), 
		*tmp_PlayerViewPointRotator.ToString()
	);*/

	FVector tmp_LineTraceEnd = tmp_PlayerViewPointLocation + (tmp_PlayerViewPointRotator.Vector() * this->LineLength);
	DrawDebugLine(GetWorld(), tmp_PlayerViewPointLocation, tmp_LineTraceEnd, FColor(FColor::Red), false, 0.f, 0.f, 10.f);

	FCollisionQueryParams TraceParamers = FCollisionQueryParams(FName(""), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		tmp_PlayerViewPointLocation,
		tmp_LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParamers
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor:%s"), *(ActorHit->GetName()));
	}
}

