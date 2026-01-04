// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UEvidenceBehavior.generated.h"

UCLASS(Abstract)
class GOCLEAN_API UEvidenceBehavior : public UObject
{
	GENERATED_BODY()


public:
	virtual void ExecuteBehavior(AActor* GhostActor) PURE_VIRTUAL(UEvidenceBehavior::Execute, );
};