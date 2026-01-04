// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaveBloodFootstep.h"

void ULeaveBloodFootstep::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("LeaveBloodFootstep Implemented"));
}