// Fill out your copyright notice in the Description page of Project Settings.


#include "GMapSystem/GDoorway.h"

#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

#include "GMapSystem/Server/GMapManager.h"
#include "GOCLEANCharacter.h"
#include "GhostBase.h"


// Sets default values
AGDoorway::AGDoorway()
{
	bReplicates = true;
	// AActor::SetReplicateMovement(true);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AGDoorway::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGDoorway::OnOverlapEnd);
	}
	else
	{
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TriggerBox->SetGenerateOverlapEvents(false);

		// TriggerBox->DestroyComponent();
	}
}

void AGDoorway::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 1. check closed
	if (bIsClosed) return;


	// 2. check other actor's effectiveness
	if (!OtherActor) return;

	ACharacter* TargetChar = Cast<ACharacter>(OtherActor);
	if (!TargetChar) return;


	// 3. check character type
	bool bIsPlayerChar;
	if (TargetChar->IsA(AGOCLEANCharacter::StaticClass()))
	{
		bIsPlayerChar = true;
	}
	else if (TargetChar->IsA(AGhostBase::StaticClass()))
	{
		bIsPlayerChar = false;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[GDoorway::TriggerEvent] Unkown type's charater detected."));
		return;
	}


	// 4. set vector
	FVector DoorForward = GetActorForwardVector();
	FVector CharVelocity = TargetChar->GetVelocity();
	
	FVector MoveDir = CharVelocity.GetSafeNormal();


	// 5. caculate dot product
	float DotResult = FVector::DotProduct(DoorForward, MoveDir);


	// 6. do process
	if (bIsPlayerChar)
	{
		if (DotResult > 0.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Player Entered to %s"), *ToZone.ToString());
			EnterPlayer();
		}
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Player Exit to %s"), *FromZone.ToString());
			ExitPlayer();
		}
	}
	else
	{
		if (DotResult > 0.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Ghost Entered to %s"), *ToZone.ToString());
			EnterGhost();
		}
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Ghost Exit to %s"), *FromZone.ToString());
			ExitGhost();
		}
	}
}

