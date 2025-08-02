// Fill out your copyright notice in the Description page of Project Settings.


#include "CloseDoor.h"

void UCloseDoor::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("CloseDoor Implemented"));
}