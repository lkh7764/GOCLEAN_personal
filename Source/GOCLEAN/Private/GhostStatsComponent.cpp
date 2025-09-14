// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostStatsComponent.h"


UGhostStatsComponent::UGhostStatsComponent()
{
	//JSH Temp
	MoveSpeed = 100.0f;
	PlayerDetectionRadius = 500.0f;
	SoundDetectionRadius = 500.0f;
	BehaviorFrequency = 3.0f;
}


float UGhostStatsComponent::GetMoveSpeed() const { return MoveSpeed; }
float UGhostStatsComponent::GetPlayerDetectionRadius() const { return PlayerDetectionRadius; }
float UGhostStatsComponent::GetSoundDetectionRadius() const { return SoundDetectionRadius; }
float UGhostStatsComponent::GetBehaviorFrequency() const { return BehaviorFrequency; }
