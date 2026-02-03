// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/GAdditionalObjFuncComponent.h"

#include "GTypes/IGInteractable.h"
#include "GObjectSystem/GNonfixedObject.h"

// Sets default values for this component's properties
UGAdditionalObjFuncComponent::UGAdditionalObjFuncComponent()
{
	
}


// Called when the game starts
void UGAdditionalObjFuncComponent::BeginPlay()
{
	Super::BeginPlay();

	
	// 1. 액터가 interactable 한지 확인
	if (IGInteractable* Interface = Cast<IGInteractable>(GetOwner()))
	{
		// 2. 액터의 델리게이트에 현재 인터랙션 기능을 등록
		if (AGNonfixedObject* OwnerActor = Cast<AGNonfixedObject>(GetOwner()))
		{
			OwnerActor->GetOnInteractionDelegate().AddUObject(this, &UGAdditionalObjFuncComponent::OnInteractionTriggered);
		}
	}
}

