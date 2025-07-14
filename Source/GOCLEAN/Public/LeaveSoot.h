// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UEvidenceBehavior.h"
#include "LeaveSoot.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API ULeaveSoot : public UEvidenceBehavior
{
	GENERATED_BODY()

public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
