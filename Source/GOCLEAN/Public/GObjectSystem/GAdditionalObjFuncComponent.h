// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GTypes/GObjectTypes.h"

#include "GAdditionalObjFuncComponent.generated.h"


UCLASS(Abstract, Blueprintable)
class GOCLEAN_API UGAdditionalObjFuncComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UGAdditionalObjFuncComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnInteractionTriggered() PURE_VIRTUAL(UGAdditionalObjFuncComponent::OnInteractionTriggered, );

public:	
	// Called every frame
	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) {}
		
};
