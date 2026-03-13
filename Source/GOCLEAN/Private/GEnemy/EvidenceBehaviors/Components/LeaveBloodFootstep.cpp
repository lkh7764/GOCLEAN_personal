#include "GEnemy/EvidenceBehaviors/Components/LeaveBloodFootstep.h"

void ULeaveBloodFootstep::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("LeaveBloodFootstep Implemented"));
}