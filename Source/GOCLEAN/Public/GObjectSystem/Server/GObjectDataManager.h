#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "GOCLEAN/Public/GObjectSystem/Server/GFixedObjectData.h"
#include "GOCLEAN/Public/GObjectSystem/Server/GNonfixedObjectData.h"
#include "GObjectDataManager.generated.h"


UCLASS()
class GOCLEAN_API UGObjectDataManager : public UWorldSubsystem
{
	GENERATED_BODY()



	// constructor
public:	
	UGObjectDataManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;



	// variables
protected:
	// 게임 플레이 중 위치이동이 발생할 수 있는 오브젝트의 정보를 저장
	UPROPERTY()
	TArray<TObjectPtr<UGNonfixedObjectData>> nonfixedObjPool;
	UPROPERTY(EditDefaultsOnly, Category = "Object Pool");
	int32 poolSize = 500;
	TArray<int32> freeObjectIndicesStack;	// pool로 반환된 가장 최신 index를 push, spawn 시 pop 하여 해당 index의 data를 재사용

	UPROPERTY()
	TMap<int32, TObjectPtr<UGNonfixedObjectData>> nonfixedObjData;	// key: objID | value: pool mapping


	TQueue<TObjectPtr<UGNonfixedObjectData>> destroyedObjs;


	// 고정 오브젝트
	//		1. 드럼통소각로: 쓰레기 타입의 비고정 오브젝트를 소각하는 용도. 고정 위치 스폰.
	UPROPERTY()
	TObjectPtr<UGFixedObjectData> fireplaceData;

	//		2. 물탱크: 물양동이를 들고 상호작용할 시, 물을 채우는 용도. 고정 위치 스폰.
	UPROPERTY()
	TObjectPtr<UGFixedObjectData> waterTankData;

	//		3. 벤딩머신: 벤딩아이템을 스폰하는 용도. 고정 위치 스폰.
	UPROPERTY()
	TObjectPtr<UGFixedObjectData> vendingMachineData;

	//		4. 캐비넷: 플레이어가 귀신을 피해 숨는 용도. 고정 위치 스폰.
	UPROPERTY()
	TArray<TObjectPtr<UGFixedObjectData>> cabinetDatas;

	//		5. 퇴마진: 퇴마를 진행하는 용도. 랜덤 위치 스폰.
	UPROPERTY()
	TObjectPtr<UGFixedObjectData> exocismCircleData;



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
