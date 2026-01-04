// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayCommonSound.h"

void UPlayCommonSound::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("PlayCommonSound Implemented"));
}