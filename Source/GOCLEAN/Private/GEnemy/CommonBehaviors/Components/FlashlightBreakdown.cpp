#include "GEnemy/CommonBehaviors/Components/FlashlightBreakdown.h"

void UFlashlightBreakdown::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("FlashlightBreakdown Implemented"));
}