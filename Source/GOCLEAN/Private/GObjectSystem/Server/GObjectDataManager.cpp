// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/Server/GObjectDataManager.h"


////////////////////////////////////////////
// DEFAULT
////////////////////////////////////////////

// Sets default values for this component's properties
UGObjectDataManager::UGObjectDataManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGObjectDataManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGObjectDataManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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