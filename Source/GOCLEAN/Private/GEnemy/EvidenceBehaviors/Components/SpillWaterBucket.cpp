#include "GEnemy/EvidenceBehaviors/Components/SpillWaterBucket.h"

void USpillWaterBucket::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("SpillWaterBucket Implemented"));
}