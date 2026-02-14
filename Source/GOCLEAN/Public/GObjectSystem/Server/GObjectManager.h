#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Net/RpcTypes.h"
#include "GObjectManager.generated.h"


class AGFixedObject;
class AGNonfixedObject;


UCLASS()
class GOCLEAN_API UGObjectManager : public UWorldSubsystem
{
	GENERATED_BODY()



	// constructor
public:	
	UGObjectManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;



protected:
	// Variables: nonfixed Object
	UPROPERTY()
	TArray<TObjectPtr<AGNonfixedObject>> NfixedObjPool;

	UPROPERTY(VisibleAnywhere, Category = "NonfixedObject Pool");
	int32 NfixedObjCnt;

	UPROPERTY(EditDefaultsOnly, Category = "NonfixedObject Pool");
	int32 PoolSize = 500;

	UPROPERTY(EditDefaultsOnly, Category = "NonfixedObject Pool");
	FVector PoolLocation = FVector(0.0, -1000.0, 0.0);

	// pool�� ��ȯ�� ���� �ֽ� index�� push, spawn �� pop �Ͽ� �ش� index�� data�� ����
	TArray<int32> FreeObjsStack;	

	UPROPERTY()
	TMap<int32, TObjectPtr<AGNonfixedObject>> NfixedObjects;	// key: IID | value: pool mapping

	// 1. Obj�� Destory ���°� �Ǹ� ��� Pool�� ������ ���� �ش� Queue�� �ִ´�.
	// 2. �ش� Queue�� �ֱ� ������ 10���� ������Ʈ�� �����Ѵ�.
	// 3. Dequeue�� index�� NFixedObj�� Map���� �����ǰ� Pool�� �����Ѵ�.
	TQueue<int32> DestroyedObjs;



	// Variables: fixed Object
	//		1. �巳��Ұ���: ������ Ÿ���� ����� ������Ʈ�� �Ұ��ϴ� �뵵. ���� ��ġ ����.
	UPROPERTY()
	TObjectPtr<AGFixedObject> Fireplace;

	//		2. ����ũ: ���絿�̸� ��� ��ȣ�ۿ��� ��, ���� ä��� �뵵. ���� ��ġ ����.
	UPROPERTY()
	TObjectPtr<AGFixedObject> WaterTank;

	//		3. �����ӽ�: ������������ �����ϴ� �뵵. ���� ��ġ ����.
	UPROPERTY()
	TObjectPtr<AGFixedObject> VendingMachine;

	//		4. ĳ���: �÷��̾ �ͽ��� ���� ���� �뵵. ���� ��ġ ����.
	UPROPERTY()
	TArray<TObjectPtr<AGFixedObject>> Cabinets;

	//		5. ����: �𸶸� �����ϴ� �뵵. ���� ��ġ ����.
	UPROPERTY()
	TObjectPtr<AGFixedObject> ExocismCircle;



	// functions
protected:
	void InitNonfixedObjects();
	void InitFixedObjects();


public:	
	// �����������Ʈ Ǯ�� ����������Ʈ�� UGObjectData�� �Ҵ� 
	void InitiateObjects(UObject*);
	// ������ �Ϸ�� ������Ʈ �����͸� 
	void SetObjectDatas();

	//UFUNCTION(BlueprintCallable)
	//AGNonfixedObject* SpawnNonfixedObject(
	//	FName TID,
	//	const FVector& Location,
	//	const FRotator& Rotation
	//);

public:
	// C -> S (���� ó��)

	// Ư�� ������Ʈ�� ���� ��ȣ�ۿ� ��û ó��
	void HandleTryInteract(class APlayerController* PC, int32 TargetInstanceId);

	// �Ұ��ο� ��⹰ ���� ��û ó��
	void HandleIncineratorThrowTrash(class APlayerController* PC, const TArray<int32>& TrashInstanceIds);

	// ĳ��� ���� ��û ó��
	void HandleCabinetEnter(class APlayerController* PC, int32 CabinetInstanceId);

	// ĳ��� ���� ��û ó��
	void HandleCabinetExit(class APlayerController* PC, int32 CabinetInstanceId);

	// ����ũ���� �� ��� ���� ��û ó��
	void HandleWaterTankStartFill(class APlayerController* PC, int32 WaterTankInstanceId);

	// �����ӽ� ������ ���� ��û ó��
	void HandleVendingSelectItem(class APlayerController* PC, FName ItemTypeId);

	// �絿�̷� �� ��� ��û ó��
	void HandleBucketPourWater(class APlayerController* PC, int32 BucketInstanceId);

	// �絿�� �� ���� ��û ó��
	void HandleBucketEmptyWater(class APlayerController* PC, int32 BucketInstanceId);

	// �絿�� ������ ���� ��û ó��
	void HandleBucketIncreaseContamination(class APlayerController* PC, int32 BucketInstanceId, int32 Delta);

	// �ٱ��Ͽ� ��⹰ ��� ��û ó��
	void HandleBasketPutTrash(class APlayerController* PC, int32 BasketInstanceId, int32 TrashParam);

	// �ٱ��� ��⹰ ���� ��û ó��
	void HandleBasketEmptyTrash(class APlayerController* PC, int32 BasketInstanceId);

	// ������Ʈ ���� ���� �غ� �Ϸ� �˸� ó��
	void HandleObjectActorSpawnReady(class APlayerController* PC, int32 ObjectInstanceId);

public:
	// S -> C (Ŭ�󿡼� ���� ó��)

	// �Ұ� �Ϸ�� ������Ʈ ó��
	void OnIncineratorTrashBurnFinished(int32 CompletedInstanceId);

	// �� ��� �Ϸ� ó��
	void OnWaterTankFillFinished(int32 WaterTankInstanceId);

	// �絿�� �� �߱� ��� ó��
	void OnBucketScoopWater(int32 BucketInstanceId);

	// ������Ʈ ���� ó��
	void OnObjectSpawned(int32 SpawnedInstanceId);

	// ������Ʈ �ı� ó��
	void OnObjectDestroyed(int32 DestroyedInstanceId);

	// ������Ʈ ���� ������ �غ� �Ϸ� ó��
	void OnObjectSpawnDataReady();

	// ��ȣ�ۿ� ���� ������Ʈ �ȳ� ó��
	void OnObjectInteractableHint(FName ObjectTypeId, int32 TargetInstanceId);

	// ��ȣ�ۿ� �ź� ��� ó��
	void OnObjectInteractionRejected(EObjectRejectReason Reason, int32 TargetInstanceId);
	

};

