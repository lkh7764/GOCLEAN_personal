#include "GEnemy/EvidenceBehaviors/Components/LeaveBloodFootstep.h"
#include "GObjectSystem/Server/GObjectManager.h"

void ULeaveBloodFootstep::ExecuteBehavior(AActor* GhostActor)
{
	if (GhostActor == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("LeaveBloodFootstep Implemented"));

	//auto ObjManager = GetWorld()->GetSubsystem<UGObjectManager>();
	//FVector Transform = GhostActor->GetActorLocation();
	//FRotator Rotation = GhostActor->GetActorRotation();

	//FTimerHandle MemberTimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &UGObjectManager::SpawnNonfixedObject, 1.0f, false);
}