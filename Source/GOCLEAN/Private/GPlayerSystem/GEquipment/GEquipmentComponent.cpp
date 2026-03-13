// Fill out your copyright notice in the Description page of Project Settings.


#include "GPlayerSystem/GEquipment/GEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include "GOCLEANSettings.h"
#include "GCharacter/GOCLEANCharacter.h"
#include "GObjectSystem/Server/GObjectManager.h"
#include "../../../GOCLEAN.h"


// Sets default values for this component's properties
UGEquipmentComponent::UGEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	InitiateEquipmentSlots();
}

void UGEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 속한 데이터 중 일부를 '나'에게만 복제하고 싶을 때
	// DOREPLIFETIME_CONDITION(UGEquipmentComponent, CurrentSlotIndex, COND_OwnerOnly);
	DOREPLIFETIME(UGEquipmentComponent, EquipmentSlots);
	DOREPLIFETIME(UGEquipmentComponent, CurrentSlotIndex);
	DOREPLIFETIME(UGEquipmentComponent, MopPollution);
	DOREPLIFETIME(UGEquipmentComponent, AutoMopPollution);
	DOREPLIFETIME(UGEquipmentComponent, PickedObjectID);
	DOREPLIFETIME(UGEquipmentComponent, PickedItemID);
}


// Called when the game starts
void UGEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AGOCLEANCharacter>(GetOwner());
}


// Called every frame
void UGEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



FName UGEquipmentComponent::GetCurrentEquipmentID()
{
	return GetEquipmentID(CurrentSlotIndex);
}

bool UGEquipmentComponent::ChangeEuquipmentInCurrSlot(FName ChangedEquipID)
{
	return ChangeEquipment(CurrentSlotIndex, ChangedEquipID);
}

bool UGEquipmentComponent::ChangeCurrentSlot(int32 ChangedSlotIndex)
{
	return ChangeCurrentSlot_Interval(CurrentSlotIndex, ChangedSlotIndex);
}


FName UGEquipmentComponent::GetEquipmentID(int32 SlotIndex)
{
	if (EquipmentSlots.Num() <= 0 || EquipmentSlots.Num() <= SlotIndex || SlotIndex < 0)
	{
		return "Error";
	}

	return EquipmentSlots[SlotIndex];
}

bool UGEquipmentComponent::ChangeEquipment(int32 SlotIndex, FName EquipID)
{
	if (EquipmentSlots.Num() <= 0 || EquipmentSlots.Num() <= SlotIndex || SlotIndex < 0)
	{
		return false;
	}

	// 나중에 유효한 equip id인지 검사하는 로직 추가 필요 -> UGDataManagerSubsystem::
	FName PrevEquipID = EquipmentSlots[SlotIndex];
	if (EquipID == "Eq_Hand")
	{
		if (SlotIndex == 0)
		{
			PickedObjectID = -1;
		}
		else if (SlotIndex == 3)
		{
			PickedItemID = -1;
		}
	}

	EquipmentSlots[SlotIndex] = EquipID;
	return true;
}

bool UGEquipmentComponent::ChangeCurrentSlot_Interval(int32 From, int32 To)
{
	if (EquipmentSlots.Num() <= 0 || EquipmentSlots.Num() <= To || To < 0)
	{
		return false;
	}

	bool Result = true;

	if (From == 0 && GetCurrentEquipmentID() == "Eq_OVariable")
	{
		// 오브젝트 매니저에서 PickedObjectID에 접근 후, 해당 오브젝트에 대하여 ChangeState 호출: Invisible -> Static
		auto* ObjectManager = GetWorld()->GetSubsystem<UGObjectManager>();
		if (!ObjectManager)
		{
			UE_LOG(LogGObject, Warning, TEXT("[ObjectManager] There's no object manager"));
			return false;
		}

		Result = ObjectManager->DropNonfixedObject(PickedObjectID);
		PickedObjectID = -1;
	}

	CurrentSlotIndex = To;

	return Result;
}



bool UGEquipmentComponent::InitiateEquipmentSlots()
{
	EquipmentSlots.Empty();

	EquipmentSlots.Add("Eq_Hand");
	EquipmentSlots.Add("Eq_Mop");
	EquipmentSlots.Add("Eq_Hand");
	EquipmentSlots.Add("Eq_Hand");

	return true;
}
