#include "GEnemy/EvidenceBehaviors/Components/TurnOffLight.h"

void UTurnOffLight::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("TurnOffLight Implemented"));
}