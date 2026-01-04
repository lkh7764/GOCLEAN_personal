// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/Server/GNonfixedObjectData.h"

// Sets default values for this component's properties
UGNonfixedObjectData::UGNonfixedObjectData()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// 1. allocate non-use state's data
	_objectID = -1;
	_objectType = -1;
	_currStatus = ENonfixedObjStatus::E_None;
	_location = FVector::ZeroVector;
}


// Called when the game starts
void UGNonfixedObjectData::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGNonfixedObjectData::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

