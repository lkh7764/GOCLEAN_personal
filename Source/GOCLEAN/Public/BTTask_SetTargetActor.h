// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetTargetActor.generated.h"

UCLASS()
class GOCLEAN_API UBTTask_SetTargetActor : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetTargetActor();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, Category = "TargetActorKey")
	struct FBlackboardKeySelector TargetActorKey;
};
