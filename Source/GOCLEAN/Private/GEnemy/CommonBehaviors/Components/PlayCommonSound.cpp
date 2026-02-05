#include "GEnemy/CommonBehaviors/Components/PlayCommonSound.h"

void UPlayCommonSound::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("PlayCommonSound Implemented"));
}