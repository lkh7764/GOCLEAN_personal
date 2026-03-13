#include "GEnemy/EvidenceBehaviors/Components/RemoveShadow.h"

void URemoveShadow::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("RemoveShadow Implemented"));
}