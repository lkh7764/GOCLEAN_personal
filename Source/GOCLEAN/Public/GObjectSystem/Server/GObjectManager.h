#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

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
	TArray<TObjectPtr<AGFixedObject>> NfixedObjPool;

	UPROPERTY(VisibleAnywhere, Category = "NonfixedObject Pool");
	int32 NfixedObjCnt;

	UPROPERTY(EditDefaultsOnly, Category = "NonfixedObject Pool");
	int32 PoolSize = 500;

	UPROPERTY(EditDefaultsOnly, Category = "NonfixedObject Pool");
	FVector PoolLocation = FVector(0.0, -1000.0, 0.0);

	// pool로 반환된 가장 최신 index를 push, spawn 시 pop 하여 해당 index의 data를 재사용
	TArray<int32> FreeObjsStack;	

	UPROPERTY()
	TMap<int32, TObjectPtr<AGFixedObject>> NfixedObjects;	// key: IID | value: pool mapping

	// 1. Obj가 Destory 상태가 되면 즉시 Pool로 보내지 말고 해당 Queue에 넣는다.
	// 2. 해당 Queue는 최근 삭제된 10개의 오브젝트만 보관한다.
	// 3. Dequeue된 index의 NFixedObj는 Map에서 삭제되고 Pool로 복귀한다.
	TQueue<int32> DestroyedObjs;



	// Variables: fixed Object
	//		1. 드럼통소각로: 쓰레기 타입의 비고정 오브젝트를 소각하는 용도. 고정 위치 스폰.
	UPROPERTY()
	TObjectPtr<AGFixedObject> Fireplace;

	//		2. 물탱크: 물양동이를 들고 상호작용할 시, 물을 채우는 용도. 고정 위치 스폰.
	UPROPERTY()
	TObjectPtr<AGFixedObject> WaterTank;

	//		3. 벤딩머신: 벤딩아이템을 스폰하는 용도. 고정 위치 스폰.
	UPROPERTY()
	TObjectPtr<AGFixedObject> VendingMachine;

	//		4. 캐비넷: 플레이어가 귀신을 피해 숨는 용도. 고정 위치 스폰.
	UPROPERTY()
	TArray<TObjectPtr<AGFixedObject>> Cabinets;

	//		5. 퇴마진: 퇴마를 진행하는 용도. 랜덤 위치 스폰.
	UPROPERTY()
	TObjectPtr<AGFixedObject> ExocismCircle;



	// functions
protected:
	void InitNonfixedObjects();
	void InitFixedObjects();


public:	
	// 비고정오브젝트 풀과 고정오브젝트에 UGObjectData를 할당 
	void InitiateObjects(UObject*);
	// 스폰이 완료된 오브젝트 데이터를 
	void SetObjectDatas();

};

