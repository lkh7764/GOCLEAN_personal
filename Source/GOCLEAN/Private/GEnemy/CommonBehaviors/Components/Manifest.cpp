#include "GEnemy/CommonBehaviors/Components/Manifest.h"

void UManifest::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("Manifest Implemented"));
}