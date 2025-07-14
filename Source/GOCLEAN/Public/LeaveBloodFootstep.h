// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UEvidenceBehavior.h"
#include "LeaveBloodFootstep.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API ULeaveBloodFootstep : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};