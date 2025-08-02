#include "GhostBase.h"
#include "PlayCommonSound.h"
#include "Manifest.h"
#include "CloseDoor.h"
#include "FlashlightBreakdown.h"
#include "PlayFootstepSound.h"

// Sets default values
AGhostBase::AGhostBase()
{
	PrimaryActorTick.bCanEverTick = true;
	// Tick(float DeltaTime) 사용 가능

	// Stats Component
	Stats = CreateDefaultSubobject<UGhostStatsComponent>(TEXT("GhostStats"));

	// JSH TODO: Player Sanity <-> SanityCorruptionRate
	SanityCorruptionRate = 10;
	BehaviorEventCycleDelay = 1.0f;
	bCanSetTimer = true;
	bIsPatrolling = false;
	bIsChasing = false;
	CurrentPatrolIndex = 0;
}

void AGhostBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = Stats->GetMoveSpeed();

	UCommonBehaviors.Add(NewObject<UPlayCommonSound>(this));
	UCommonBehaviors.Add(NewObject<UManifest>(this));
	UCommonBehaviors.Add(NewObject<UCloseDoor>(this));
	UCommonBehaviors.Add(NewObject<UFlashlightBreakdown>(this));
	UCommonBehaviors.Add(NewObject<UPlayFootstepSound>(this));
}

// JSH TODO: Exception Handling+
// Behavior Event Defination
void AGhostBase::PerformBehaviorEvent()
{
	int32 RandEventNum;
	int32 RandEvidenceBehaviorNum;
	int32 RandCommonBehaviorNum;
	RandEventNum = FMath::RandRange(0, 8);

	UE_LOG(LogTemp, Warning, TEXT("---------------------------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("RandEventNumResult: %d"), RandEventNum);

	if (0 <= RandEventNum && RandEventNum <= 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Perform Evidence Behavior]"));
		
		RandEvidenceBehaviorNum = FMath::RandRange(0, UEvidenceBehaviors.Num() - 1);
		if (UEvidenceBehaviors[RandEvidenceBehaviorNum] == nullptr) return;
		UEvidenceBehaviors[RandEvidenceBehaviorNum]->ExecuteBehavior(this);
	}
	else if (4 <= RandEventNum && RandEventNum <= 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Perform Common Behavior]"));
		
		RandCommonBehaviorNum = FMath::RandRange(0, UCommonBehaviors.Num() - 1);
		if (UCommonBehaviors[RandCommonBehaviorNum] == nullptr) return;
		UCommonBehaviors[RandCommonBehaviorNum]->ExecuteBehavior(this);
	}
	else if (6 <= RandEventNum && RandEventNum <= 7)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Sustain Patrol State]"));
	}
	else if (RandEventNum == 8)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Stop]"));
	}
}

void AGhostBase::CheckBehaviorEventCondition()
{
	if (SanityCorruptionRate <= 10 && bCanSetTimer) {
		GetWorldTimerManager().SetTimer(GhostBehaviorCycleHandle, this, &AGhostBase::PerformBehaviorEvent, BehaviorEventCycleDelay, true);
		bCanSetTimer = false;
	}
	else if (SanityCorruptionRate > 10 && !bCanSetTimer) {
		GetWorldTimerManager().ClearTimer(GhostBehaviorCycleHandle);
		bCanSetTimer = true;
	}
	else return;
}

// Enrage Event Declaration

void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckBehaviorEventCondition();
}

void AGhostBase::StartPatrolEvent()
{
	;
}

void AGhostBase::StartEnrageEvent(AActor* Target)
{
	bIsEnraged = true;
	TargetActor = Target;
}