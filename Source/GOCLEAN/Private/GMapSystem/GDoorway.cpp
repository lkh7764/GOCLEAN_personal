// Fill out your copyright notice in the Description page of Project Settings.


#include "GMapSystem/GDoorway.h"

#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

#include "GMapSystem/Server/GMapManager.h"
#include "GCharacter/GOCLEANCharacter.h"
#include "GEnemy/Base/GhostBase.h"
#include "GOCLEANSettings.h"



// constructor
AGDoorway::AGDoorway()
{
	bReplicates = true;
	// AActor::SetReplicateMovement(true);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}



// life cycle & engine event
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
			EnterPlayer(OtherActor);
		}
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Player Exit to %s"), *FromZone.ToString());
			ExitPlayer(OtherActor);
		}
	}
	else
	{
		if (DotResult > 0.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Ghost Entered to %s"), *ToZone.ToString());
			EnterGhost(OtherActor);
		}
		{
			UE_LOG(LogTemp, Log, TEXT("[GDoorway::TriggerEvent] Ghost Exit to %s"), *FromZone.ToString());
			ExitGhost(OtherActor);
		}
	}
}


// game event
void AGDoorway::EnterPlayer(AActor* PlayerActor)
{
	// 1. find player's session index and check effectiveness
	int32 PlayerIndex = FindPlayerSessionIndex(PlayerActor);
	if (PlayerIndex < 0 && PlayerIndex >= UGOCLEANSettings::Get()->MaxParticipant)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GDoorway::TriggerEvent] Cannot found matched player in session list"));
		return;
	}


	// 2. remove player index at FromZone's included player list


	// 3. add player index at ToZone's included player list
}
void AGDoorway::ExitPlayer(AActor* PlayerActor)
{
	// 1. find player's session index and check effectiveness
	int32 PlayerIndex = FindPlayerSessionIndex(PlayerActor);
	if (PlayerIndex < 0 && PlayerIndex >= UGOCLEANSettings::Get()->MaxParticipant)
	{
		UE_LOG(LogTemp, Warning, TEXT("[GDoorway::TriggerEvent] Cannot found matched player in session list"));
		return;
	}


	// 2. remove player index at ToZone's included player list


	// 3. add player index at FromZone's included player list
}

void AGDoorway::EnterGhost(AActor* GhostActor)
{
	// if parameter does not use, remove parameter

	// 1. set FromZone's bIncludeGhost to FALSE


	// 2. set ToZone's bIncludeGhost to TRUE
}
void AGDoorway::ExitGhost(AActor* GhostActor)
{
	// if parameter does not use, remove parameter

	// 1. set ToZone's bIncludeGhost to FALSE


	// 2. set FromZone's bIncludeGhost to TRUE
}

int32 AGDoorway::FindPlayerSessionIndex(AActor* PlayerActor)
{
	int32 Index = -1;


	// 1. find same player character in server's session manager


	// 2. get the player's index


	return Index;
}
