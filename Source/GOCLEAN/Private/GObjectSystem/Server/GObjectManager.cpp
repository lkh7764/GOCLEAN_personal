// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/Server/GObjectManager.h"

#include "../../../GOCLEAN.h"
#include "GObjectSystem/GNonfixedObject.h"
#include "GObjectSystem/GFixedObject.h"

#include "GCharacter/GOCLEANCharacter.h"
#include "GPlayerSystem/InteractionComponent.h"
#include "GPlayerSystem/GEquipment/GEquipmentComponent.h"
#include "GDataManagerSubsystem.h"
#include "GTypes/IGInteractable.h"

#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"


////////////////////////////////////////////
// DEFAULT
////////////////////////////////////////////

// Sets default values for this component's properties
UGObjectManager::UGObjectManager()
{
    NfixedObjCnt = 0;
}

// Called when the game starts
void UGObjectManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);


	// 1. 앵커 액터 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PoolRoot = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), PoolLocation, FRotator::ZeroRotator, SpawnParams);
    ActiveRoot = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

#if WITH_EDITOR
    PoolRoot->SetActorLabel(TEXT("-- OBJECT_POOL_ROOT --"));
    ActiveRoot->SetActorLabel(TEXT("-- ACTIVE_OBJECT_ROOT --"));
#endif


    InitiateObjects();
}
// Called when the game starts
void UGObjectManager::Deinitialize()
{
	Super::Deinitialize();

	// ...
	
}



////////////////////////////////////////////
// INITIATE & ALLOCATE
////////////////////////////////////////////

AGNonfixedObject* UGObjectManager::SpawnNewEmptyNonfixedObject()
{
    auto NewActor = GetWorld()
        ->SpawnActor<AGNonfixedObject>(AGNonfixedObject::StaticClass(), PoolLocation, FRotator::ZeroRotator);

    return NewActor;
}

void UGObjectManager::ReturnToPool(AGNonfixedObject* Actor)
{
    if (!Actor) return;

    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);

    Actor->AttachToActor(PoolRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    NfixedObjPool.Push(Actor);
}

AGNonfixedObject* UGObjectManager::GetFromPool()
{
    if (NfixedObjPool.Num() > 0)
    {
        return NfixedObjPool.Pop();
    }

    return nullptr;
}

AGNonfixedObject* UGObjectManager::SpawnNonfixedObject(
    FName TID,
    ENonfixedObjState SpawnState,
    const FVector& Location,
    const FRotator& Rotation)
{
    // 1. 오브젝트 풀에서 오브젝트 가져오기
    auto Target = GetFromPool();

    // 2. 오브젝트 풀에 가져올 오브젝트가 없다면 새로 생성
    if (!Target)
    {
        Target = SpawnNewEmptyNonfixedObject();
    }

    if (Target)
    {
        // 3. 위치 및 상태 초기화
        Target->SetActorLocationAndRotation(Location, Rotation);

        Target->SetActorHiddenInGame(false);
        Target->SetActorEnableCollision(true); 
        
        Target->AttachToActor(ActiveRoot, FAttachmentTransformRules::KeepWorldTransform);


        // 4. 스폰 데이터 세팅
        NfixedObjCnt++;

        FGNonfixedObjData InitData = {
            NfixedObjCnt,
            TID,
            Location,
            Rotation,
            SpawnState,
            true
        };

        Target->SetObjectData(InitData);
    }

    return Target;
}

// 비고정오브젝트 풀과 고정오브젝트에 UGObjectData를 할당 
void UGObjectManager::InitNonfixedObjects(int32 CreatedPoolSize)
{
	NfixedObjPool.Empty(PoolSize);
	FreeObjsStack.Empty();
	NfixedObjects.Empty();

	for (int32 i = 0; i < CreatedPoolSize; ++i)
	{
		auto NewActor = SpawnNewEmptyNonfixedObject();
		if (NewActor)
		{
            ReturnToPool(NewActor);
		}
	}
}

void UGObjectManager::InitiateObjects()
{
#if PROTOTYPE_2026_02
    // 맵에 배치된 오브젝트 중 NonfixedObj 타입의 오브젝트를 로드해옴
    FindAllNonfixedObjects();

    // 예비 오브젝트 풀을 생성
    InitNonfixedObjects(50);
#else
    // 1. 오브젝트풀 생성
    InitNonfixedObjects(PoolSize);
#endif


    // finish obj load
}

void UGObjectManager::FindAllNonfixedObjects()
{
    UE_LOG(LogObjectPool, Warning, TEXT("[Prototype] 맵에 배치된 NonfixedObject를 수집합니다."));

    int32 FoundCnt = 0;
    for (TActorIterator<AGNonfixedObject> It(GetWorld()); It; ++It)
    {
        AGNonfixedObject* PlacedObj = *It;

        if (PlacedObj)
        {
            FoundCnt++;

            NfixedObjects.Add(FoundCnt, PlacedObj);

            PlacedObj->AttachToActor(ActiveRoot, FAttachmentTransformRules::KeepWorldTransform);
            PlacedObj->UpdateObjectData();

            FoundCnt++;
        }
    }


    UE_LOG(LogObjectPool, Log, TEXT("[Prototype] 총 %d개의 NonfixedObject를 맵에서 찾았습니다!"), FoundCnt);

    NfixedObjCnt += FoundCnt;
}




////////////////////////////////////////////
// RPC: C to S
////////////////////////////////////////////

void UGObjectManager::HandleTryInteract(APlayerController* PC, int32 TargetInstanceId)
{
    if (!PC) return;


    APawn* Pawn = PC->GetPawn();
    if (!Pawn) return;

    UE_LOG(LogTemp, Warning, TEXT("[ObjectManager] Player %s tried to interact with object %d"),  *PC->GetName(), TargetInstanceId);
    

    // 인터렉션 로직 추가
    AGOCLEANCharacter* PlayerChar = Cast<AGOCLEANCharacter>(Pawn);
    if (!PlayerChar) return;

    UGEquipmentComponent* EquipComp = PlayerChar->GetEquipComp();
    if (!PlayerChar->GetInteractionComp() || !EquipComp) return;

    // get equip id
    FName EquipID = EquipComp->GetCurrentEquipmentID();


    // PlayerChar->Server_TryInteraction(EquipID);
    UObject* Target = PlayerChar->GetInteractionComp()->GetCurrentTarget();
    if (Target)
    {
        IGInteractable* Interactable = Cast<IGInteractable>(Target);
        if (Interactable)
        {
            if (Interactable->CanInteract(EquipID, PlayerChar))
            {
                Interactable->ExecuteInteraction(EquipID, PlayerChar);
                UE_LOG(LogObject, Log, TEXT("[GCharacter] Interaction Executed on %s"), *Target->GetName());
            }
        }
    }
}

void UGObjectManager::HandleIncineratorThrowTrash(APlayerController* PC, const TArray<int32>& TrashInstanceIds)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Incinerator_ThrowTrash by %s, Count=%d"), PC ? *PC->GetName() : TEXT("NULL"), TrashInstanceIds.Num());

    //  서버에서 인스턴스ID들 유효성 검사 + 소각로에 투입 처리
}

void UGObjectManager::HandleCabinetEnter(APlayerController* PC, int32 CabinetInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Cabinet_Enter by %s, CabinetIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), CabinetInstanceId);

    // 캐비닛 사용 중 / 숨기 처리 + 거리 체크
}

void UGObjectManager::HandleCabinetExit(APlayerController* PC, int32 CabinetInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Cabinet_Exit by %s, CabinetIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), CabinetInstanceId);

    // 캐비닛 사용 해제 처리
}

void UGObjectManager::HandleWaterTankStartFill(APlayerController* PC, int32 WaterTankInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] WaterTank_StartFill by %s, TankIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), WaterTankInstanceId);

    //  물 담기 시작 (서버에서 판정 후 완료 시 S2C 알림 필요)
}

void UGObjectManager::HandleVendingSelectItem(APlayerController* PC, FName ItemTypeId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Vending_SelectItem by %s, ItemType=%s"), PC ? *PC->GetName() : TEXT("NULL"), *ItemTypeId.ToString());

    // 선택 카운트/스폰 조건 처리
}

void UGObjectManager::HandleBucketPourWater(APlayerController* PC, int32 BucketInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Bucket_PourWater by %s, BucketIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), BucketInstanceId);

    //  쏟기 처리
}

void UGObjectManager::HandleBucketEmptyWater(APlayerController* PC, int32 BucketInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Bucket_EmptyWater by %s, BucketIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), BucketInstanceId);

    //  비우기 처리
}

void UGObjectManager::HandleBucketIncreaseContamination(APlayerController* PC, int32 BucketInstanceId, int32 Delta)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Bucket_IncreaseContamination by %s, BucketIID=%d, Delta=%d"), PC ? *PC->GetName() : TEXT("NULL"), BucketInstanceId, Delta);

    //  오염도 증가 처리
}

void UGObjectManager::HandleBasketPutTrash(APlayerController* PC, int32 BasketInstanceId, int32 TrashParam)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Basket_PutTrash by %s, BasketIID=%d, TrashParam=%d"), PC ? *PC->GetName() : TEXT("NULL"), BasketInstanceId, TrashParam);

    //  담기 처리
}

void UGObjectManager::HandleBasketEmptyTrash(APlayerController* PC, int32 BasketInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Basket_EmptyTrash by %s, BasketIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), BasketInstanceId);

    //  비우기 처리
}

void UGObjectManager::HandleObjectActorSpawnReady(APlayerController* PC, int32 ObjectInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[C2S] Object_ActorSpawnReady by %s, ObjIID=%d"), PC ? *PC->GetName() : TEXT("NULL"), ObjectInstanceId);

    //  클라 준비 완료 처리
}



////////////////////////////////////////////
// RPC: S to C
////////////////////////////////////////////

void UGObjectManager::OnIncineratorTrashBurnFinished(int32 CompletedInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] Incinerator_TrashBurnFinished IID=%d"), CompletedInstanceId);
    //  클라 연출/UI/로컬 처리
}

void UGObjectManager::OnWaterTankFillFinished(int32 WaterTankInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] WaterTank_FillFinished TankIID=%d"), WaterTankInstanceId);
}

void UGObjectManager::OnBucketScoopWater(int32 BucketInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] Bucket_ScoopWater BucketIID=%d"), BucketInstanceId);
}

void UGObjectManager::OnObjectSpawned(int32 SpawnedInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] Object_Spawned IID=%d"), SpawnedInstanceId);
}

void UGObjectManager::OnObjectDestroyed(int32 DestroyedInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] Object_Destroyed IID=%d"), DestroyedInstanceId);
}

void UGObjectManager::OnObjectSpawnDataReady()
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] Object_SpawnDataReady"));
}

void UGObjectManager::OnObjectInteractableHint(FName ObjectTypeId, int32 TargetInstanceId)
{
    UE_LOG(LogTemp, Log, TEXT("[S2C] Object_InteractableHint Type=%s, TargetIID=%d"), *ObjectTypeId.ToString(), TargetInstanceId);
}

void UGObjectManager::OnObjectInteractionRejected(EObjectRejectReason Reason, int32 TargetInstanceId)
{
    UE_LOG(LogTemp, Warning, TEXT("[S2C] Object_InteractionRejected Reason=%d, TargetIID=%d"), (int32)Reason, TargetInstanceId);
}