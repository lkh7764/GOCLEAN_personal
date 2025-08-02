// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostAIController.generated.h"

UCLASS()
class GOCLEAN_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	FTimerHandle CheckArrivalHandle;

	virtual void BeginPlay() override;

	void MoveToPatrolPoint();
	void CheckArrival();


	// 지금와서 보면, 굳이 BB와 BT를 써야할까?
	void RunGhostBT();
	UPROPERTY(EditDefaultsOnly, Category = "Ghost|AI")

	UBehaviorTree* BT;
	UBlackboardComponent* BB;
};