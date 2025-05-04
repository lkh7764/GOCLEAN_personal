// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAIController.generated.h"

UCLASS()
class GOCLEAN_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGhostAIController();

	virtual void BeginPlay() override;

	void RunGhostBT();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ghost|AI")
	UBehaviorTree* BT;

	UBlackboardComponent* BB;
};