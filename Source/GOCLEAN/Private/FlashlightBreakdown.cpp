// Fill out your copyright notice in the Description page of Project Settings.


#include "FlashlightBreakdown.h"

void UFlashlightBreakdown::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("FlashlightBreakdown Implemented"));
}