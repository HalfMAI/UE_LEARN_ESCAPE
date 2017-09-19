// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "TimerManager.h"

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
	this->CurrentTriggerMass = 0.f;

	GetWorld()->GetTimerManager().SetTimer(
		this->OpenDoorCheckTimerHandler,
		this,
		&UOpenDoor::PrintMass,
		1.0f,
		true
	);

	this->Owner = GetOwner();
	this->ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

	ensureMsgf(this->PressurePlate, TEXT("PressurePlate NOT FOUND!!"));
}

void UOpenDoor::OpenDoor()
{
	if (this->IsDoorClosed == false)
	{
		return;
	}

	this->IsDoorClosed = false;

	this->OnOpenDoorEvent.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	if (this->IsDoorClosed == true)
	{
		return;
	}

	this->IsDoorClosed = true;
	this->OnCloseDoorEvent.Broadcast();
}

bool UOpenDoor::IsDoorCanOpen()
{
	this->CurrentTriggerMass = 0.f;
	TArray<AActor*> tmpOverlapeActors;
	this->PressurePlate->GetOverlappingActors(OUT tmpOverlapeActors);
	for (const auto& tmpActor : tmpOverlapeActors)
	{
		this->CurrentTriggerMass += tmpActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	if (this->CurrentTriggerMass >= this->TriggerMass)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UOpenDoor::PrintMass()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Trigger Mass:%f"), this->CurrentTriggerMass);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (this->IsDoorCanOpen())
	{
		this->OpenDoor();
	}
	else
	{
		this->CloseDoor();
	}
}

