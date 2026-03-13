#include "GEnemy/EvidenceBehaviors/Components/RestoreWaste.h"

void URestoreWaste::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("RestoreWaste Implemented"));
}