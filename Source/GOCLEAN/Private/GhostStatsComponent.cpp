// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostStatsComponent.h"


UGhostStatsComponent::UGhostStatsComponent()
{
	;
}



// Getter, Setter //
float UGhostStatsComponent::GetMoveSpeed() const { return MoveSpeed; }
float UGhostStatsComponent::GetPlayerDetectionRadius() const { return PlayerDetectionRadius; }
float UGhostStatsComponent::GetSoundDetectionRadius() const { return SoundDetectionRadius; }
float UGhostStatsComponent::GetBehaviorFrequency() const { return BehaviourFrequency; }
