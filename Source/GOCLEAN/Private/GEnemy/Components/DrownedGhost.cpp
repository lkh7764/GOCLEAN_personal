#include "GEnemy/Components/DrownedGhost.h"
#include "AIController.h"
#include "GEnemy/EvidenceBehaviors/Components/LeaveBloodFootstep.h"
#include "GEnemy/EvidenceBehaviors/Components/LeaveSoot.h"
#include "GEnemy/EvidenceBehaviors/Components/LeaveFrost.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ADrownedGhost::ADrownedGhost()
{
	// MoveSpeed, PlayerDetectionRadius, SoundDetectionRadius, BehaviourFrequency
	StatsComp->InitializeGhostStats(100.0f, 500.0f, 500.0f, 3.0f);
}

void ADrownedGhost::BeginPlay()
{
	Super::BeginPlay();

	EvidenceBehaviors.Add(NewObject<ULeaveBloodFootstep>(this));
	EvidenceBehaviors.Add(NewObject<ULeaveSoot>(this));
	EvidenceBehaviors.Add(NewObject<ULeaveFrost>(this));
}