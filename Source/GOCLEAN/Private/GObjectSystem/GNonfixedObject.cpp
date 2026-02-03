// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/GNonfixedObject.h"

#include "Components/StaticMeshComponent.h"

// Sets default values
AGNonfixedObject::AGNonfixedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGNonfixedObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGNonfixedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


bool AGNonfixedObject::CanInteract() const
{
    return true;
}

// 2. ExecuteInteraction ±¸Çö
void AGNonfixedObject::ExecuteInteraction()
{
    TriggerInteraction();
}

