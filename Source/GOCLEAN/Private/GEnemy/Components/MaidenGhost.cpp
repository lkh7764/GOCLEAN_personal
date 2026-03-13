#include "GEnemy/Components/MaidenGhost.h"
#include "GEnemy/EvidenceBehaviors/Components/LeaveBloodFootstep.h"
#include "GEnemy/EvidenceBehaviors/Components/LeaveFrost.h"
#include "GEnemy/EvidenceBehaviors/Components/SpillWaterBucket.h"
#include "GEnemy/EvidenceBehaviors/Components/RestoreWaste.h"

AMaidenGhost::AMaidenGhost()
{
	// MoveSpeed, PlayerDetectionRadius, SoundDetectionRadius, BehaviourFrequency
	StatsComp->InitializeGhostStats(300.0f, 1000.0f, 700.0f, 3.0f);
}

void AMaidenGhost::BeginPlay()
{
	Super::BeginPlay();

	EvidenceBehaviors.Add(NewObject<ULeaveBloodFootstep>(this));
	EvidenceBehaviors.Add(NewObject<ULeaveFrost>(this));
}