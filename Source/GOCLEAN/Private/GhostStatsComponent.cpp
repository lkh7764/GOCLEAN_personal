// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostStatsComponent.h"

// Sets default values for this component's properties
UGhostStatsComponent::UGhostStatsComponent()
{
	// 나중에 Date table 파씽 예정
	MoveSpeed = 100.0f;
	PlayerDetectionRadius = 20.0f;
	SoundDetectionRadius = 20.0f;
	BehaviorFrequency = 3;
}

int UGhostStatsComponent::GetMoveSpeed()
{
	return MoveSpeed;
}
