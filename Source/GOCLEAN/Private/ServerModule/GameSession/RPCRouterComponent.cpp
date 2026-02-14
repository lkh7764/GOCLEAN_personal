// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerModule/GameSession/RPCRouterComponent.h"
#include <ServerModule/GameSession/GameSessionState.h>
#include <GObjectSystem/Server/GObjectManager.h>


#include <GTypes/GObjectTypes.h>
#include <GObjectSystem/GNonfixedObjCoreComponent.h>
#include <Kismet/GameplayStatics.h>
#include "GObjectSystem/GNonfixedObject.h"


// Sets default values for this component's properties
URPCRouterComponent::URPCRouterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPCRouterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URPCRouterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URPCRouterComponent::Server_ObjectEvent_Implementation( EObjectEvent_C2S EventType, const FObjectPayload_C2S& Payload)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
        return;

    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC) return;


    UWorld* World = GetWorld();
    AGameSessionState* GS = World ? World->GetGameState<AGameSessionState>() : nullptr;
    if (!GS) return;

    UGObjectManager* OM = GS->GetObjectManager();
    if (!OM) return;



    switch (EventType)
    {
    case EObjectEvent_C2S::Object_TryInteract:
        OM->HandleTryInteract(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::Incinerator_ThrowTrash:
        OM->HandleIncineratorThrowTrash(PC, Payload.TargetInstanceIds);
        break;

    case EObjectEvent_C2S::Cabinet_Enter:
        OM->HandleCabinetEnter(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::Cabinet_Exit:
        OM->HandleCabinetExit(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::WaterTank_StartFill:
        OM->HandleWaterTankStartFill(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::Vending_SelectItem:
        OM->HandleVendingSelectItem(PC, Payload.ItemTypeId);
        break;

    case EObjectEvent_C2S::Bucket_PourWater:
        OM->HandleBucketPourWater(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::Bucket_EmptyWater:
        OM->HandleBucketEmptyWater(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::Bucket_IncreaseContamination:
        OM->HandleBucketIncreaseContamination(PC, Payload.TargetInstanceId, Payload.ParamInt);
        break;

    case EObjectEvent_C2S::Basket_PutTrash:
        OM->HandleBasketPutTrash(PC, Payload.TargetInstanceId, Payload.ParamInt);
        break;

    case EObjectEvent_C2S::Basket_EmptyTrash:
        OM->HandleBasketEmptyTrash(PC, Payload.TargetInstanceId);
        break;

    case EObjectEvent_C2S::Object_ActorSpawnReady:
        OM->HandleObjectActorSpawnReady(PC, Payload.TargetInstanceId);
        break;


    default:
        break;
    }
}

void URPCRouterComponent::Client_ObjectEvent_Implementation(EObjectEvent_S2C EventType, const FObjectPayload_S2C& Payload)
{
    UWorld* World = GetWorld();
    AGameSessionState* GS = World ? World->GetGameState<AGameSessionState>() : nullptr;
    if (!GS) return;

    UGObjectManager* OM = GS->GetObjectManager();
    if (!OM) return;

    switch (EventType)
    {
    case EObjectEvent_S2C::Incinerator_TrashBurnFinished:
        OM->OnIncineratorTrashBurnFinished(Payload.TargetInstanceId);
        break;

    case EObjectEvent_S2C::WaterTank_FillFinished:
        OM->OnWaterTankFillFinished(Payload.TargetInstanceId);
        break;

    case EObjectEvent_S2C::Bucket_ScoopWater:
        OM->OnBucketScoopWater(Payload.TargetInstanceId);
        break;

    case EObjectEvent_S2C::Object_Spawned:
        OM->OnObjectSpawned(Payload.TargetInstanceId);
        break;

    case EObjectEvent_S2C::Object_Destroyed:
        OM->OnObjectDestroyed(Payload.TargetInstanceId);
        break;

    case EObjectEvent_S2C::Object_SpawnDataReady:
        OM->OnObjectSpawnDataReady();
        break;

    case EObjectEvent_S2C::Object_InteractableHint:
        OM->OnObjectInteractableHint(Payload.ObjectTypeId, Payload.TargetInstanceId);
        break;

    case EObjectEvent_S2C::Object_InteractionRejected:
        OM->OnObjectInteractionRejected(Payload.RejectReason, Payload.TargetInstanceId);
        break;

    default:
        break;
    }
}