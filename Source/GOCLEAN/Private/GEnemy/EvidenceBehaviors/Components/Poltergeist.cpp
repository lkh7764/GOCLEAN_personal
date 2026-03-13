#include "GEnemy/EvidenceBehaviors/Components/Poltergeist.h"

void UPoltergeist::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Poltergeist Implemented"));
}