// Fill out your copyright notice in the Description page of Project Settings.

#include "GObjectSystem/GNonfixedObject.h"

#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

#include "GTypes/DataTableRow/GObjectDataRow.h"
#include "GDataManagerSubsystem.h"
#include "GObjectSystem/GNonfixedObjCoreComponent.h"



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
		RootPrimitive->SetSimulatePhysics(true);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Ignore);

		SetActorHiddenInGame(false);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
		}

		break;

	case ENonfixedObjState::E_Kinematic:
		RootPrimitive->SetSimulatePhysics(true);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Ignore);

		SetActorHiddenInGame(false);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
		}

		break;

	case ENonfixedObjState::E_Fixed:
		RootPrimitive->SetSimulatePhysics(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootPrimitive->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Ignore);

		SetActorHiddenInGame(false);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			InteractionVolume->SetCollisionResponseToChannel(ECC_GInteractable, ECR_Block);
		}

		break;

	case ENonfixedObjState::E_Invisible:
		RootPrimitive->SetSimulatePhysics(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetActorHiddenInGame(true);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Disintegrating:
		RootPrimitive->SetSimulatePhysics(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetActorHiddenInGame(false);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Destroyed:
		RootPrimitive->SetSimulatePhysics(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetActorHiddenInGame(true);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	case ENonfixedObjState::E_Temporary:
		RootPrimitive->SetSimulatePhysics(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetActorHiddenInGame(true);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;

	default:
		RootPrimitive->SetSimulatePhysics(false);

		RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SetActorHiddenInGame(true);

		if (InteractionVolume)
		{
			InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		break;
	}
}

