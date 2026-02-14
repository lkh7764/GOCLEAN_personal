// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/GNonfixedObjCoreComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGNonfixedObjCoreComponent::UGNonfixedObjCoreComponent()
{
	SetIsReplicatedByDefault(true);
	PrimaryComponentTick.bCanEverTick = false;
}

void UGNonfixedObjCoreComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGNonfixedObjCoreComponent, IID);
	DOREPLIFETIME(UGNonfixedObjCoreComponent, TID);
	DOREPLIFETIME(UGNonfixedObjCoreComponent, InteractionCnt);
	DOREPLIFETIME(UGNonfixedObjCoreComponent, State);
}


// Called when the game starts
void UGNonfixedObjCoreComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGNonfixedObjCoreComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// interface
bool UGNonfixedObjCoreComponent::CanInteract(FName EquipID) const
{
	// check equip id

	return true;
}

void UGNonfixedObjCoreComponent::ExecuteInteraction(FName EquipID)
{
	if (!GetOwner()->HasAuthority())
		return;

	InteractionCnt++;
	// InteractionCnt에 따른 동작은 아래 델리게이트에서
	TriggerInteraction();

	UE_LOG(LogTemp, Log, TEXT("[GEquipment] %s Interacted! Count: %d, Tool: %s"), *GetName(), InteractionCnt, *EquipID.ToString());
}


void UGNonfixedObjCoreComponent::OnRep_InteractionCnt()
{
	// update visual
}

void UGNonfixedObjCoreComponent::OnRep_State()
{
	// update visual
}
