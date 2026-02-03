// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/Server/GObjectManager.h"


////////////////////////////////////////////
// DEFAULT
////////////////////////////////////////////

// Sets default values for this component's properties
UGObjectManager::UGObjectManager()
{
	// 
}


// Called when the game starts
void UGObjectManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// ...
	
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

// 비고정오브젝트 풀과 고정오브젝트에 UGObjectData를 할당 
void UGObjectManager::InitNonfixedObjects()
{
	// nonfixedObjPool.Empty(poolSize);
	freeObjectIndicesStack.Empty();
	// nonfixedObjData.Empty();

	for (int32 i = 0; i < poolSize; ++i)
	{
		//UGNonfixedObjectData* newData = NewObject<UGNonfixedObjectData>(this);
		//if (newData)
		//{
		//	nonfixedObjPool.Add(newData);
		//	freeObjectIndicesStack.Add(i);
		//}
	}
}
void UGObjectManager::InitiateObjects(UObject* WorldContextObject)
{
	// 1. 오브젝트풀 생성
	InitNonfixedObjects();
}