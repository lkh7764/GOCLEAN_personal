// Fill out your copyright notice in the Description page of Project Settings.


#include "Manifest.h"

void UManifest::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Manifest Implemented"));
}