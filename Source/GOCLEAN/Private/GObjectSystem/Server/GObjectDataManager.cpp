// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/Server/GObjectDataManager.h"


////////////////////////////////////////////
// DEFAULT
////////////////////////////////////////////

// Sets default values for this component's properties
UGObjectDataManager::UGObjectDataManager()
{
	// 
}


// Called when the game starts
void UGObjectDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// ...
	
}
// Called when the game starts
void UGObjectDataManager::Deinitialize()
{
	Super::Deinitialize();

	// ...
	
}



////////////////////////////////////////////
// INITIATE & ALLOCATE
////////////////////////////////////////////

// 비고정오브젝트 풀과 고정오브젝트에 UGObjectData를 할당 
void UGObjectDataManager::InitNonfixedObjects()
{
	nonfixedObjPool.Empty(poolSize);
	freeObjectIndicesStack.Empty();
	nonfixedObjData.Empty();

	for (int32 i = 0; i < poolSize; ++i)
	{
		UGNonfixedObjectData* newData = NewObject<UGNonfixedObjectData>(this);
		if (newData)
		{
			nonfixedObjPool.Add(newData);
			freeObjectIndicesStack.Add(i);
		}
	}
}
void UGObjectDataManager::InitiateObjects(UObject* WorldContextObject)
{
	// 1. 오브젝트풀 생성
	InitNonfixedObjects();
}