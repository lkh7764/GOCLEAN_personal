// Fill out your copyright notice in the Description page of Project Settings.

#include "GObjectSystem/GNonfixedObject.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

#include "GTypes/DataTableRow/GObjectDataRow.h"
#include "GDataManagerSubsystem.h"
#include "GObjectSystem/GNonfixedObjCoreComponent.h"
#include "GCharacter/GOCLEANCharacter.h"



AGNonfixedObject::AGNonfixedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bReplicates = true;
	SetReplicateMovement(true);


	RootPrimitive = CreateDefaultSubobject<UStaticMeshComponent>("RootPrimitive");
	SetRootComponent(RootPrimitive);


	InteractionVolume = CreateDefaultSubobject<UBoxComponent>("InteractionVolume");
	InteractionVolume->SetupAttachment(RootPrimitive);

	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractionVolume->SetHiddenInGame(false);

	InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
	InteractionVolume->SetSimulatePhysics(false);


	CoreComp = CreateDefaultSubobject<UGNonfixedObjCoreComponent>("CoreComponent");

}

void AGNonfixedObject::UpdateInteractionBounds()
{
	if (!RootPrimitive) return;

	FBoxSphereBounds Bounds = RootPrimitive->Bounds;

	InteractionVolume->SetBoxExtent(Bounds.BoxExtent);
	InteractionVolume->SetRelativeLocation(FVector(0, 0, Bounds.BoxExtent.Z));
}


void AGNonfixedObject::BeginPlay()
{
	Super::BeginPlay();

	UpdateInteractionBounds();
}


void AGNonfixedObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AGNonfixedObject::UpdateVisualByState()
{
	if (!CoreComp) return;
	if (!RootPrimitive) return;

	switch (CoreComp->GetNonfixedObjState())
	{
	case ENonfixedObjState::E_Static:

		SetActorHiddenInGame(false);

		break;


	case ENonfixedObjState::E_Picked:

		SetActorHiddenInGame(false);

		break;

	case ENonfixedObjState::E_Kinematic:

		SetActorHiddenInGame(false);

		break;

	case ENonfixedObjState::E_Fixed:
		
		SetActorHiddenInGame(false);

		break;

	case ENonfixedObjState::E_Invisible:

		SetActorHiddenInGame(true);

		break;

	case ENonfixedObjState::E_Disintegrating:

		SetActorHiddenInGame(false);

		break;

	case ENonfixedObjState::E_Destroyed:

		SetActorHiddenInGame(true);

		break;

	case ENonfixedObjState::E_Temporary:

		SetActorHiddenInGame(true);

		break;

	default:

		SetActorHiddenInGame(true);

		break;
	}
}
void AGNonfixedObject::UpdatePhysicsByState()
{
	if (!CoreComp) return;
	if (!RootPrimitive) return;

	switch (CoreComp->GetNonfixedObjState())
	{
	case ENonfixedObjState::E_Static:
		RootPrimitive->SetSimulatePhysics(true);
		RootPrimitive->SetEnableGravity(true);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Ignore);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
		}

		break;


	case ENonfixedObjState::E_Picked:
		RootPrimitive->SetSimulatePhysics(false);
		RootPrimitive->SetEnableGravity(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Kinematic:
		RootPrimitive->SetSimulatePhysics(true);
		RootPrimitive->SetEnableGravity(true);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Ignore);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
		}

		break;

	case ENonfixedObjState::E_Fixed:
		RootPrimitive->SetSimulatePhysics(false);
		RootPrimitive->SetEnableGravity(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Ignore);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
		}

		break;

	case ENonfixedObjState::E_Invisible:
		RootPrimitive->SetSimulatePhysics(false);
		RootPrimitive->SetEnableGravity(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Disintegrating:
		RootPrimitive->SetSimulatePhysics(true);
		RootPrimitive->SetEnableGravity(true);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionObjectType(EEC_GIncinerator);
		RootPrimitive->SetCollisionResponseToAllChannels(ECR_Ignore);
		RootPrimitive->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		RootPrimitive->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		RootPrimitive->SetCollisionResponseToChannel(EEC_GIncinerator, ECR_Block);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Destroyed:
		RootPrimitive->SetSimulatePhysics(false);
		RootPrimitive->SetEnableGravity(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Temporary:
		RootPrimitive->SetSimulatePhysics(false);
		RootPrimitive->SetEnableGravity(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	default:
		RootPrimitive->SetSimulatePhysics(false);
		RootPrimitive->SetEnableGravity(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;
	}
}



void AGNonfixedObject::UpdateObjectData(int32 IID)
{
	if (!CoreComp) return;

	CoreComp->IID = IID;
}

void AGNonfixedObject::Multicast_OnPickedUp_Implementation(AGOCLEANCharacter* TargetCharacter)
{
	if (!TargetCharacter) return;

	// 1. 모든 클라이언트에서 물리를 강제로 끕니다. (가장 중요!)
	GetStaticMeshComp()->SetSimulatePhysics(false);
	GetStaticMeshComp()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 2. 모든 클라이언트에서 캐릭터 손에 붙입니다.
	// 서버가 복제해주길 기다리지 않고 직접 붙여버리는 겁니다.
	AttachToComponent(TargetCharacter->GetHandMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Charater001_Bip001-R-HandSocket"));

	// 3. 리더님이 아까 만든 트랜스폼 보정 함수도 여기서 호출!
	TargetCharacter->SetHeldObjectRelativeTransform(this);
}

