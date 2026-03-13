#include "GEnemy/CommonBehaviors/Components/PlayFootstepSound.h"

void UPlayFootstepSound::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("PlayFootstepSound Implemented"));
}