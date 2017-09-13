// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


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

	this->IsDoorClosed = true;
	this->LastOpenDoorTime = 999.0f;

	this->Owner = GetOwner();
	this->ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	if (this->IsDoorClosed == false)
	{
		return;
	}

	this->IsDoorClosed = false;
	this->Owner->SetActorRotation(FRotator(0.0f, OpenDoorAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	if (this->IsDoorClosed == true)
	{
		return;
	}

	this->IsDoorClosed = true;
	this->Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (this->PressurePlate->IsOverlappingActor(this->ActorThatOpens))
	{
		this->OpenDoor();

		this->LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - this->LastOpenDoorTime > this->OpenDoorDelay)
	{
		this->CloseDoor();
	}
}

