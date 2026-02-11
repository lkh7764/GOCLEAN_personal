// Fill out your copyright notice in the Description page of Project Settings.

#include "GObjectSystem/GNonfixedObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GTypes/IGInteractable.h"

// Sets default values
AGNonfixedObject::AGNonfixedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);


	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);

	InteractionVolume->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);

	InteractionCnt = 0;
	State = ENonfixedObjState::E_Static;
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
	OnStateUpdate();
}


bool AGNonfixedObject::Initialize(const FGNonfixedObjData& NewData)
{
	TID = NewData.TID;

	// update properties

	return true;
}



bool AGNonfixedObject::CanInteract(FName EquipID) const
{
	// check equip id

    return true;
}

void AGNonfixedObject::ExecuteInteraction(FName EquipID)
{
	InteractionCnt++;
	UE_LOG(LogTemp, Log, TEXT("[GEquipment] %s Interacted! Count: %d, Tool: %s"), *GetName(), InteractionCnt, *EquipID.ToString());

	// InteractionCnt에 따른 동작은 아래 델리게이트에서
    TriggerInteraction();
}

void AGNonfixedObject::OnStateUpdate()
{
	// 상태별 업데이트 로직
}

void AGNonfixedObject::CleanUp() {}
bool AGNonfixedObject::ChangeState(ENonfixedObjState ChangedState) { State = ChangedState; return true; }
void AGNonfixedObject::OnStateExit() {}
void AGNonfixedObject::OnStateEnter() {}

