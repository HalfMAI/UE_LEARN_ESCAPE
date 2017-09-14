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
	SetupPhysiscHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->PhysicsHandle->GrabbedComponent)
	{
		FVector tmpLineStartLocation;
		FVector tmpLineEndLocation;
		this->GetLineTraceStartEndLocation(tmpLineStartLocation, tmpLineEndLocation);

		this->PhysicsHandle->SetTargetLocation(tmpLineEndLocation);
	}
}

void UGrabber::GetLineTraceStartEndLocation(FVector& out_StartLocation, FVector& out_EndLoaction) const
{
	FVector tmp_PlayerViewPointLocation;
	FRotator tmp_PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT tmp_PlayerViewPointLocation,
		OUT tmp_PlayerViewPointRotator
	);

	FVector tmp_LineTraceEnd = tmp_PlayerViewPointLocation + (tmp_PlayerViewPointRotator.Vector() * this->LineLength);

	out_StartLocation = tmp_PlayerViewPointLocation;
	out_EndLoaction = tmp_LineTraceEnd;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector tmp_PlayerViewPointLocation;
	FVector tmp_LineTraceEnd;
	this->GetLineTraceStartEndLocation(tmp_PlayerViewPointLocation, tmp_LineTraceEnd);
	
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

	return Hit;
}

void UGrabber::GrabObj()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key PRESSED~"));

	FHitResult tmpHitResult = this->GetFirstPhysicsBodyInReach();
	AActor* tmpHitActor = tmpHitResult.GetActor();
	UPrimitiveComponent* tmpHitComponent = tmpHitResult.GetComponent();
	if (tmpHitActor)
	{
		this->PhysicsHandle->GrabComponent(
			tmpHitComponent,
			NAME_None,
			tmpHitActor->GetActorLocation(),
			true
		);
	}
}

void UGrabber::ReleaseObj()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key RELEASED~"));

	this->PhysicsHandle->ReleaseComponent();
}

void UGrabber::SetupPhysiscHandleComponent()
{
	this->PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!this->PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: PhysicsHandle NOT FOUND!"), *(GetOwner()->GetName()));
	}
	else
	{

	}
}

void UGrabber::SetupInputComponent()
{
	this->InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!this->InputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: InputComponent NOT FOUND!"), *(GetOwner()->GetName()));
	}
	else
	{
		this->InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabObj);
		this->InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseObj);
	}
}