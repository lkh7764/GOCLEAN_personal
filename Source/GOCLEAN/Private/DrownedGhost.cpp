#include "DrownedGhost.h"
#include "AIController.h"
#include "LeaveBloodFootstep.h"
#include "LeaveSoot.h"
#include "LeaveFrost.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ADrownedGhost::ADrownedGhost()
{
	
	

	//if (Stats == nullptr) return;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ADrownedGhost::BeginPlay()
{
	Super::BeginPlay();

	UEvidenceBehaviors.Add(NewObject<ULeaveBloodFootstep>(this));
	UEvidenceBehaviors.Add(NewObject<ULeaveSoot>(this));
	UEvidenceBehaviors.Add(NewObject<ULeaveFrost>(this));
}