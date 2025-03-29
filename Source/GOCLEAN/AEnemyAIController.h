// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API AEnemyAIController : public AEnemyAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	void RunAI();
	void StopAI();

protected:

	virtual void OnPosses(APawn* InPawn) override;

private:

	TObjectPtr<class UBlackboardData> BBAsset;

	TObjectPtr<class UBehaviorTree> BTAsset;
};
