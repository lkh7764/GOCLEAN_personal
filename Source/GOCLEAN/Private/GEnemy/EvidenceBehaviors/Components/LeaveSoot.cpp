#include "GEnemy/EvidenceBehaviors/Components/LeaveSoot.h"

void ULeaveSoot::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("LeaveSoot Implemented"));
}