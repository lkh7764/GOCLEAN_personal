// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayFootstepSound.h"

void UPlayFootstepSound::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("PlayFootstepSound Implemented"));
}