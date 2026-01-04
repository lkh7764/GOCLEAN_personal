// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonBehavior.h"
#include "CloseDoor.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API UCloseDoor : public UCommonBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
