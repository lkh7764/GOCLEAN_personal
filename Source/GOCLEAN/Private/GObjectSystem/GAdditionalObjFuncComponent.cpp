// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/GAdditionalObjFuncComponent.h"

#include "GTypes/IGInteractable.h"
#include "GTypes/DataTableRow/GObjectDataRow.h"
#include "GObjectSystem/GNonfixedObjCoreComponent.h"
#include "GObjectSystem/GNonfixedObject.h"
#include "GObjectSystem/GFixedObject.h"
#include "GCharacter/GOCLEANCharacter.h"
#include "GPlayerSystem/GEquipment/GEquipmentComponent.h"
#include "GDataManagerSubsystem.h"
#include "GObjectSystem/Server/GObjectManager.h"
#include "../../GOCLEAN.h"

#include "Kismet/GameplayStatics.h"



UGAdditionalObjFuncComponent::UGAdditionalObjFuncComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UGAdditionalObjFuncComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogGObject, Log, TEXT("[AdditionalFuncComp] Begin Play %s: Add delegate funcs"), *GetOwner()->GetName());

	TArray<UActorComponent*> InteractComps =
		GetOwner()->GetComponentsByInterface(UGInteractable::StaticClass());
	if (InteractComps.Num() > 0)
	{
		if (UGNonfixedObjCoreComponent* CoreComp = Cast<UGNonfixedObjCoreComponent>(InteractComps[0]))
		{
			CoreComp->GetOnInteractionDelegate().AddUObject(this, &UGAdditionalObjFuncComponent::OnInteractionTriggered);
			CoreComp->GetOnStateChangedDelegate().AddUObject(this, &UGAdditionalObjFuncComponent::OnStateChangeTriggered);
		}
	}
}



UGPickComponent::UGPickComponent()
{

}

void UGPickComponent::InitializeAdditionalData(const FGNonfixedObjData& Data)
{
	Super::InitializeAdditionalData(Data);

	bIsPickedUp = false;
	OwnerPlayer = nullptr;
}

void UGPickComponent::BeginPlay() 
{
	Super::BeginPlay();
}

void UGPickComponent::OnInteractionTriggered(AGOCLEANCharacter* Target)
{
	PickUpObject(Target);
}

void UGPickComponent::OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState)
{
	if (PrevState == ENonfixedObjState::E_Picked && ChangedState == ENonfixedObjState::E_Static)
	{
		DropObject();
	}
}

void UGPickComponent::PickUpObject(AGOCLEANCharacter* Target)
{
	if (Target->GetEquipComp()->GetCurrentEquipmentID() != "Eq_Hand") return;

	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (Owner && Owner->GetNonfixedObjCoreComp() && GetWorld())
	{
		bIsPickedUp = true;
		OwnerPlayer = Target;

		Owner->GetNonfixedObjCoreComp()->ChangeState(ENonfixedObjState::E_Picked);
		Target->SetHeldObject(Owner);

		UGDataManagerSubsystem* DataManager = UGDataManagerSubsystem::Get(GetWorld());
		const FGObjectDataRow* Data =
			DataManager ? DataManager->GetObjectData(Owner->GetNonfixedObjCoreComp()->TID) : nullptr;

		if (Data)
		{
			FName PickedEquipID = Data->PickedEquipID;
			Target->GetEquipComp()->ChangeEuquipmentInCurrSlot(PickedEquipID);
		}

		Owner->Multicast_OnPickedUp(Target);
	}
}

void UGPickComponent::DropObject()
{
	if (!OwnerPlayer) return;

	AGNonfixedObject* OwnerActor = Cast<AGNonfixedObject>(GetOwner());
	if (!OwnerActor) return;


	// get spawn data from owner player
	float DropDistance = 100.0f;
	FVector DropLocation = OwnerPlayer->GetActorLocation() + (OwnerPlayer->GetActorForwardVector() * DropDistance);

	DropLocation.Z = OwnerPlayer->GetActorLocation().Z;


	// set owner actor's
	OwnerActor->SetActorLocation(DropLocation);
	OwnerActor->SetActorRotation(OwnerPlayer->GetActorRotation());


	// set owner player's
	OwnerPlayer->DropHeldObject();


	bIsPickedUp = false;
	OwnerPlayer = nullptr;
}



UGRemovingComponent::UGRemovingComponent()
{

}

void UGRemovingComponent::InitializeAdditionalData(const FGNonfixedObjData& Data)
{
	Super::InitializeAdditionalData(Data);

	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (Owner && Owner->GetNonfixedObjCoreComp() && GetWorld())
	{
		Owner->GetNonfixedObjCoreComp()->ChangeState(ENonfixedObjState::E_Invisible);

		UGDataManagerSubsystem* DataManager = UGDataManagerSubsystem::Get(GetWorld());
		const FGObjectDataRow* Data =
			DataManager ? DataManager->GetObjectData(Owner->GetNonfixedObjCoreComp()->TID) : nullptr;

		if (Data)
		{
			InteractionMaxCnt = Data->InteractionFinCnt;
		}
	}
}

void UGRemovingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGRemovingComponent::OnInteractionTriggered(AGOCLEANCharacter* Target)
{
	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (Owner && Owner->GetNonfixedObjCoreComp())
	{
		if (Owner->GetNonfixedObjCoreComp()->InteractionCnt < InteractionMaxCnt)
		{
			SetVisualByInteractionCnt(Owner);
		}
		else
		{
			SetDestroyThisObject(Owner);
		}
	}
}

void UGRemovingComponent::SetVisualByInteractionCnt(AGNonfixedObject* Owner)
{
	UGDataManagerSubsystem* DataManager = UGDataManagerSubsystem::Get(GetWorld());
	const FGObjectDataRow* Data =
		DataManager ? DataManager->GetObjectData(Owner->GetNonfixedObjCoreComp()->TID) : nullptr;

	if (Data)
	{
		if (Data->Category == EGObjectCategory::E_Filth)
		{
			// 데칼타입의 경우, 메시의 알파값을 감소

			// 물걸레의 오염도를 증가
		}
		else if (Data->Category == EGObjectCategory::E_Trash_B)
		{
			// 큰 쓰레기의 경우, 1/2 달성 시 broken 메시 활성화
		}
	}
}

void UGRemovingComponent::SetDestroyThisObject(AGNonfixedObject* Owner)
{
	Owner->GetNonfixedObjCoreComp()->ChangeState(ENonfixedObjState::E_Destroyed);
}




UGMultiInteractionComponent::UGMultiInteractionComponent()
{

}

void UGMultiInteractionComponent::InitializeAdditionalData(const FGNonfixedObjData& Data)
{
	Super::InitializeAdditionalData(Data);
}

void UGMultiInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}




UGBurningCompopnent::UGBurningCompopnent()
{

}

void UGBurningCompopnent::InitializeAdditionalData(const FGNonfixedObjData& Data)
{
	Super::InitializeAdditionalData(Data);
}

void UGBurningCompopnent::BeginPlay()
{
	Super::BeginPlay();
}

void UGBurningCompopnent::OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState)
{
	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (!Owner) return;

	if (PrevState == ENonfixedObjState::E_Picked && ChangedState == ENonfixedObjState::E_Disintegrating)
	{
		StartBurning(Owner);
	}

	else if (PrevState == ENonfixedObjState::E_Disintegrating && ChangedState != ENonfixedObjState::E_Destroyed)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(BurnTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(BurnTimerHandle);
		}
	}
}

void UGBurningCompopnent::StartBurning(AGNonfixedObject* Owner)
{
	if (!Owner || !GetWorld()) return;


	// set object's transform
	auto ObjectManager = GetWorld()->GetSubsystem<UGObjectManager>();
	if (ObjectManager && ObjectManager->GetIncineratorActor())
	{
		AGFixedObject* Incinerator = ObjectManager->GetIncineratorActor();


		FBox Bounds = Incinerator->GetComponentsBoundingBox();

		FVector DropLocation = Incinerator->GetActorLocation();
		DropLocation.Z = Bounds.Max.Z + 10.f;

		
		Owner->SetActorLocation(DropLocation);
		Owner->SetActorRotation(FRotator::ZeroRotator);


		Incinerator->OnCustomEvent_Bool(true);
	}


	FVector NewScale = Owner->GetActorScale3D() * 0.45f;
	Owner->SetActorScale3D(NewScale);


	// start timer
	GetWorld()->GetTimerManager().SetTimer(
		BurnTimerHandle,
		this,
		&UGBurningCompopnent::OnBurnTimerFinished,
		BurningInterval,
		false
	);
}

void UGBurningCompopnent::OnBurnTimerFinished()
{
	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (Owner && Owner->GetNonfixedObjCoreComp())
	{
		Owner->GetNonfixedObjCoreComp()->ChangeState(ENonfixedObjState::E_Destroyed);
	}
}




UGSpawnerCompopnent::UGSpawnerCompopnent()
{

}

void UGSpawnerCompopnent::InitializeAdditionalData(const FGNonfixedObjData& Data)
{
	Super::InitializeAdditionalData(Data);
}

void UGSpawnerCompopnent::BeginPlay()
{
	Super::BeginPlay();
}

void UGSpawnerCompopnent::OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState)
{
	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (Owner) return;

	if (ChangedState == ENonfixedObjState::E_Destroyed)
	{
		SpawnDerivedObject(Owner);
	}
}

void UGSpawnerCompopnent::SpawnDerivedObject(AGNonfixedObject* Owner)
{
	UWorld* World = GetWorld();
	if (!World) return;

	auto* DataManager = UGDataManagerSubsystem::Get(World);
	auto* Data = DataManager ? DataManager->GetObjectData(Owner->GetNonfixedObjCoreComp()->TID) : nullptr;
	if (!Data) return;

	auto* ObjectManager = World->GetSubsystem<UGObjectManager>();
	if (ObjectManager)
	{
		FVector SpawnLocation = Owner->GetActorLocation();
		FRotator SpawnRotation = Owner->GetActorRotation();

		for (int32 i = 0; i < Data->DerivedObjCnt; ++i)
		{
			ObjectManager->SpawnNonfixedObject(
				Data->DerivedObjID, 
				ENonfixedObjState::E_Static,
				SpawnLocation,
				SpawnRotation);
		}
	}
}




UGInteractSoundCompopnent::UGInteractSoundCompopnent()
{
	CachedInteractSound = nullptr;
}

void UGInteractSoundCompopnent::InitializeAdditionalData(const FGNonfixedObjData& Data)
{
	Super::InitializeAdditionalData(Data);

	AGNonfixedObject* Owner = Cast<AGNonfixedObject>(GetOwner());
	if (Owner && Owner->GetNonfixedObjCoreComp() && GetWorld())
	{
		// 2. 데이터 매니저에서 TID 기반 데이터 가져오기
		UGDataManagerSubsystem* DataManager = UGDataManagerSubsystem::Get(GetWorld());
		const FGObjectDataRow* DataRow = DataManager ? DataManager->GetObjectData(Owner->GetNonfixedObjCoreComp()->TID) : nullptr;

		// 3. 사운드 에셋 캐싱
		if (DataRow && DataRow->InteractSoundAsset)
		{
			CachedInteractSound = DataRow->InteractSoundAsset;
		}
	}
}

void UGInteractSoundCompopnent::BeginPlay()
{
	Super::BeginPlay();
}

void UGInteractSoundCompopnent::OnInteractionTriggered(AGOCLEANCharacter* Target)
{
	if (CachedInteractSound && GetOwner())
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			CachedInteractSound,
			GetOwner()->GetActorLocation()
		);
	}
}
