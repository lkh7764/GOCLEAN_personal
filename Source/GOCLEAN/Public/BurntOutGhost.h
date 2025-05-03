// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GhostBase.h"
#include "BurntOutGhost.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API ABurntOutGhost : public AGhostBase
{
	GENERATED_BODY()
	

public:
	ABurntOutGhost();

	virtual void BeginPlay() override;

	virtual void StartEnragedChase(AActor* Target) override;

	virtual void PlayCommonSound() override;

	virtual void Manifest() override;

	virtual void Patrol() {};
};
