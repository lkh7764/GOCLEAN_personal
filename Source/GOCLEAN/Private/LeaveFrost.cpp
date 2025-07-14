// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaveFrost.h"

void ULeaveFrost::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("LeaveFrost Implemented"));
}