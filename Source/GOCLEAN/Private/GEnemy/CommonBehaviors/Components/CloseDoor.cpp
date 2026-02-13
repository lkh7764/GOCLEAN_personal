#include "GEnemy/CommonBehaviors/Components/CloseDoor.h"

void UCloseDoor::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("CloseDoor Implemented"));
}