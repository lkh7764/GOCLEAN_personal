#include "GEnemy/EvidenceBehaviors/Components/LeaveFrost.h"

void ULeaveFrost::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("LeaveFrost Implemented"));
}