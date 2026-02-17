// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/GFixedObject.h"
#include "GObjectSystem/Server/GObjectManager.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AGFixedObject::AGFixedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGFixedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGFixedObject, State);
}

// Called when the game starts or when spawned
void AGFixedObject::BeginPlay()
{
	Super::BeginPlay();
	
    if (auto* ObjectManager = GetWorld()->GetSubsystem<UGObjectManager>())
    {
        ObjectManager->RegisterFixedObject(TID, this);
    }
}

// Called every frame
void AGFixedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGFixedObject::ChangeState(EGFixedObjState ChangedState)
{
	auto PrevState = State;
	if (PrevState == ChangedState) return;

	OnStateExit(PrevState);
	OnStateEnter(ChangedState);

	State = ChangedState;
	OnRep_State();
}

void AGFixedObject::OnRep_State()
{
	OnStateChanged_Visual();
}
