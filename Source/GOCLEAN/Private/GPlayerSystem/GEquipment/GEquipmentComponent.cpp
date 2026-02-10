// Fill out your copyright notice in the Description page of Project Settings.


#include "GPlayerSystem/GEquipment/GEquipmentComponent.h"
#include "Net/UnrealNetwork.h"
#include "GOCLEANSettings.h"
#include "GCharacter/GOCLEANCharacter.h"


// Sets default values for this component's properties
UGEquipmentComponent::UGEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);


}
void UGEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 속한 데이터 중 일부를 '나'에게만 복제하고 싶을 때
	// DOREPLIFETIME_CONDITION(UGEquipmentComponent, CurrentSlotIndex, COND_OwnerOnly);
	DOREPLIFETIME(UGEquipmentComponent, EquipmentSlots);
	DOREPLIFETIME(UGEquipmentComponent, CurrentSlotIndex);
}


// Called when the game starts
void UGEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



//bool AGOCLEANCharacter::InitiateEquipMeshPool()
//{
//	// call in constructor
//
//
//	int32 TempSlots = 5;
//	// init skeletal mesh
//	for (int i = 0; i < TempSlots; ++i)
//	{
//		FString Compname = FString::Printf(TEXT("SlotMeshComp_%d"), i);
//		USkeletalMeshComponent* NewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(*Compname);
//
//		if (NewMesh)
//		{
//			// character로 이동 후, Onwer 빼기
//			NewMesh->SetupAttachment(Onwer->GetMesh(), TEXT("socket_name"));
//			NewMesh->SetHiddenInGame(true);
//		}
//	}
//
//	return true;
//}

