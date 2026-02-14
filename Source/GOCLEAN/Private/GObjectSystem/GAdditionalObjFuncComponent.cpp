// Fill out your copyright notice in the Description page of Project Settings.


#include "GObjectSystem/GAdditionalObjFuncComponent.h"

#include "GTypes/IGInteractable.h"
#include "GObjectSystem/GNonfixedObjCoreComponent.h"

// Sets default values for this component's properties
UGAdditionalObjFuncComponent::UGAdditionalObjFuncComponent()
{
	
}


// Called when the game starts
void UGAdditionalObjFuncComponent::BeginPlay()
{
	Super::BeginPlay();


	TArray<UActorComponent*> InteractComps =
		GetOwner()->GetComponentsByInterface(UGInteractable::StaticClass());
	if (InteractComps.Num() > 0)
	{
		if (UGNonfixedObjCoreComponent* CoreComp = Cast<UGNonfixedObjCoreComponent>(GetOwner()))
		{
			CoreComp->GetOnInteractionDelegate().AddUObject(this, &UGAdditionalObjFuncComponent::OnInteractionTriggered);
		}
	}
}

