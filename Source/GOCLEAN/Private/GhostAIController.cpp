// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GhostBase.h"

void AGhostAIController::BeginPlay()
{
	Super::BeginPlay();

	RunGhostBT();

	MoveToPatrolPoint();
}

void AGhostAIController::RunGhostBT()
{
	// !!!! BeginPlay()에서 호출한 게 문제였음.
	BB = GetBlackboardComponent();
	if(UseBlackboard(BT->BlackboardAsset, BB)) RunBehaviorTree(BT);
}

void AGhostAIController::MoveToPatrolPoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Move to next patrol point"));
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr || GhostCharacter->PatrolPoints.Num() == 0) return;

	AActor* TargetPoint = GhostCharacter->PatrolPoints[GhostCharacter->CurrentPatrolIndex];
	if (TargetPoint == nullptr) return;

	MoveToActor(TargetPoint);

	GetWorld()->GetTimerManager().SetTimer(CheckArrivalHandle, this, &AGhostAIController::CheckArrival, 3.0f, true);
}

// Chase 추가 시 bool 판단
void AGhostAIController::CheckArrival()
{
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr || GhostCharacter->PatrolPoints.Num() == 0) return;

	AActor* TargetPoint = GhostCharacter->PatrolPoints[GhostCharacter->CurrentPatrolIndex];
	if (TargetPoint == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPoint->GetActorLocation());
	if (Distance < 50.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckArrivalHandle);

		GhostCharacter->CurrentPatrolIndex = (GhostCharacter->CurrentPatrolIndex + 1) % (GhostCharacter->PatrolPoints.Num());
		MoveToPatrolPoint();
	}
}