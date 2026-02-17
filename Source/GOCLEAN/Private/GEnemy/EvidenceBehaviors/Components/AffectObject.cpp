#include "GEnemy/EvidenceBehaviors/Components/AffectObject.h"

void UAffectObject::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("AffectObject Implemented"));
}