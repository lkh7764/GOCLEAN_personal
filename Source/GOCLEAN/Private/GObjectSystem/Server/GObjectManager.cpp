// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/Server/GObjectManager.h"

#include "../../../GOCLEAN.h"
#include "GObjectSystem/GNonfixedObject.h"
#include "GObjectSystem/GNonfixedObjCoreComponent.h"
#include "GObjectSystem/GFixedObject.h"

#include "GCharacter/GOCLEANCharacter.h"
#include "GPlayerSystem/InteractionComponent.h"
#include "GPlayerSystem/GEquipment/GEquipmentComponent.h"
#include "GDataManagerSubsystem.h"
#include "GTypes/IGInteractable.h"

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
}

// Called when the game starts
void UGObjectManager::Deinitialize()
{
	Super::Deinitialize();

	// ...
	
}

bool UGObjectManager::ShouldCreateSubsystem(UObject* Outer) const
{
    if (!Super::ShouldCreateSubsystem(Outer)) return false;

    UWorld* World = Outer->GetWorld();
    if (World)
    {
        return World->GetNetMode() < NM_Client;
    }

    return false;
}

void UGObjectManager::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    InitiateObjects();
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
void UGObjectManager::CreateNonfixedObjPool(int32 CreatedPoolSize)
{
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
    NfixedObjPool.Empty(PoolSize);
    FreeObjsStack.Empty();
    NfixedObjects.Empty();

#if PROTOTYPE_2026_02
    // 맵에 배치된 오브젝트 중 NonfixedObj 타입의 오브젝트를 로드해옴
    FindAllNonfixedObjects();

    // 예비 오브젝트 풀을 생성
    CreateNonfixedObjPool(50);

#else
    // 1. 오브젝트풀 생성
    CreateNonfixedObjPool(PoolSize);
#endif


    // finish obj load
}

void UGObjectManager::FindAllNonfixedObjects()
{
    UE_LOG(LogObjectPool, Warning, TEXT("[Prototype] Find spawned NonfixedObjects"));

    int32 FoundCnt = 0;
    for (TActorIterator<AGNonfixedObject> It(GetWorld()); It; ++It)
    {
        AGNonfixedObject* PlacedObj = *It;

        if (PlacedObj)
        {
            FoundCnt++;

            NfixedObjects.Add(FoundCnt, PlacedObj);

            PlacedObj->AttachToActor(ActiveRoot, FAttachmentTransformRules::KeepWorldTransform);
            PlacedObj->UpdateObjectData(FoundCnt);
        }
    }


    UE_LOG(LogObjectPool, Log, TEXT("[Prototype] Found %d number of NonfixedObjects!"), FoundCnt);

    NfixedObjCnt += FoundCnt;
}

bool UGObjectManager::DropNonfixedObject(int32 PickedObjectIID)
{
    AGNonfixedObject* DroppedObj = GetNonfixedObject(PickedObjectIID);
    if (!DroppedObj)
    {
        UE_LOG(LogGObject, Log, TEXT("[GObjectManager] Cannot found %d order object"), PickedObjectIID);
        return false;
    }

    auto* CoreComp = DroppedObj->GetNonfixedObjCoreComp();
    if (!CoreComp)
    {
        UE_LOG(LogGObject, Log, TEXT("[GObjectManager] Cannot found %d order object's core component"), PickedObjectIID);
        return false;
    }

    CoreComp->ChangeState(ENonfixedObjState::E_Static);
    return true;
}



////////////////////////////////////////////
// Fixed Object
////////////////////////////////////////////
void UGObjectManager::RegisterFixedObject(FName TID, AGFixedObject* Target)
{
    if (TID == "Obj_Incinerator")
    {
        if (!Incinerator)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] Incinerator can not spawned over two"));
            return;
        }

        Incinerator = Target;
    }
    else if (TID == "Obj_WaterTank")
    {
        if (!WaterTank)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] WaterTank can not spawned over two"));
            return;
        }

        WaterTank = Target;
    }
    else if (TID == "Obj_MagicFloor")
    {
        ExocismCircle.Add(Target);
    }
    else if (TID == "Obj_CBasketSpawner")
    {
        if (!BasketSpawner)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] BasketSpawner can not spawned over two"));
            return;
        }

        BasketSpawner = Target;
    }
    else if (TID == "Obj_CBucketSpawner")
    {
        if (!BucketSpawner)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] BucketSpawner can not spawned over two"));
            return;
        }

        BucketSpawner = Target;
    }
    else if (TID == "Obj_VendingMachine")
    {
        if (!VendingMachine)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] VendingMachine can not spawned over two"));
            return;
        }

        VendingMachine = Target;
    }
    else if (TID == "Obj_TBowlSpawner")
    {
        if (!TBowlSpawner)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] TBowlSpawner can not spawned over two"));
            return;
        }

        TBowlSpawner = Target;
    }
    else if (TID == "Obj_TAmuletSpawner")
    {
        if (!TAmuletSpawner)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] TAmuletSpawner can not spawned over two"));
            return;
        }

        TAmuletSpawner = Target;
    }
    else if (TID == "Obj_TPileSpawner")
    {
        if (!TPileSpawner)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] TPileSpawner can not spawned over two"));
            return;
        }

        TPileSpawner = Target;
    }
    else if (TID == "Obj_Cabinet")
    {
        Cabinets.Add(Target);
    }
    else if (TID == "Obj_CCTV")
    {
        if (!CCTV)
        {
            UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] CCTV can not spawned over two"));
            return;
        }

        CCTV = Target;
    }
    else
    {
        UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] Could not found atched variable: %s"), *Target->GetName());
    }

    UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] Complete register successly!: %s - %d"), *Target->GetName(), Target->GetInstanceID());
}

AGFixedObject* UGObjectManager::ActiveRandomExocismCircle()
{
    if (ExocismCircle.IsEmpty())
    {
        UE_LOG(LogGObject, Warning, TEXT("[GFixedObject] Exocism circle is empty!"));
        return nullptr;
    }

    AGFixedObject* SelectedCircle;
    while (true)
    {
        SelectedCircle = ExocismCircle[FMath::RandRange(0, ExocismCircle.Num() - 1)];

        if (SelectedCircle->GetState() == EGFixedObjState::E_Invisible)
        {
            UE_LOG(LogGObject, Log, TEXT("[GFixedObject] Found exocism circle to active!"));
            break;
        }
    }

    UpdateExocismCircleStates(SelectedCircle);

    return SelectedCircle;
}

void UGObjectManager::UpdateExocismCircleStates(AGFixedObject* ActiveCircle)
{
    if (!ActiveCircle) return;

    for (auto Circle : ExocismCircle)
    {
        if (Circle == ActiveCircle) continue;
        Circle->ChangeState(EGFixedObjState::E_Invisible);
    }

    ActiveCircle->ChangeState(EGFixedObjState::E_Static);
}




////////////////////////////////////////////
// RPC: C to S
////////////////////////////////////////////

void UGObjectManager::HandleTryInteract(APlayerController* PC, int32 TargetInstanceId)
{
    if (!PC) return;


    APawn* Pawn = PC->GetPawn();
    if (!Pawn) return;

    UE_LOG(LogGObject, Warning, TEXT("[ObjectManager] Player %s tried to interact with object %d"),  *PC->GetName(), TargetInstanceId);
    

    // 인터렉션 로직 추가
    AGOCLEANCharacter* PlayerChar = Cast<AGOCLEANCharacter>(Pawn);
    if (!PlayerChar) return;

    UGEquipmentComponent* EquipComp = PlayerChar->GetEquipComp();
    if (!PlayerChar->GetInteractionComp() || !EquipComp) return;

    // get equip id
    FName EquipID = EquipComp->GetCurrentEquipmentID();
    if (EquipID == "Error")
    {
        UE_LOG(LogGObject, Warning, TEXT("[GObject] Equip Error!"));
    }
    else if (EquipID == "Eq_OVariable")
    {
        int32 PickedObjectIID = PlayerChar->GetEquipComp()->GetPickedObjectID();

        DropNonfixedObject(PickedObjectIID);
    }
    else
    {
        UObject* Target = PlayerChar->GetInteractionComp()->GetCurrentTarget();
        if (Target)
        {
            IGInteractable* Interactable = Cast<IGInteractable>(Target);
            if (Interactable)
            {
                if (Interactable->CanInteract(EquipID, PlayerChar))
                {
                    Interactable->ExecuteInteraction(EquipID, PlayerChar);
                    UE_LOG(LogGObject, Log, TEXT("[GCharacter] Interaction Executed on %s"), *Target->GetName());
                }
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