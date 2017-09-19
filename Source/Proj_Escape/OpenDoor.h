// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnOpenDoorEvent );
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseDoorEvent);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJ_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

	bool IsDoorCanOpen();
	void PrintMass();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
private:
	AActor* Owner;
	AActor* ActorThatOpens;
	bool IsDoorClosed = true;

	float CurrentTriggerMass = 0.0f;
	FTimerHandle OpenDoorCheckTimerHandler;


	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;
	
	UPROPERTY(EditAnywhere)
	float TriggerMass = 50.0f;

	UPROPERTY(BlueprintAssignable)
	FOnOpenDoorEvent OnOpenDoorEvent;
	UPROPERTY(BlueprintAssignable)
	FOnCloseDoorEvent OnCloseDoorEvent;
};
