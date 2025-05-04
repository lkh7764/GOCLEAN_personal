#include "BTTask_SetTargetActor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GOCLEAN/GOCLEANCharacter.h"

UBTTask_SetTargetActor::UBTTask_SetTargetActor()
{
	NodeName = "Set Target Actor";

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetTargetActor, TargetActorKey), ACharacter::StaticClass());
}

EBTNodeResult::Type UBTTask_SetTargetActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{ 
	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;
	
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(World, 0);
	
	if (!Player) return EBTNodeResult::Failed;

	//JSH Flag : Casting
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;
	
	BB->SetValueAsObject(TargetActorKey.SelectedKeyName, Player);

	return EBTNodeResult::Succeeded;
}
