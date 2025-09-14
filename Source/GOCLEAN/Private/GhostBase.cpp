#include "GhostBase.h"
#include "PlayCommonSound.h"
#include "Manifest.h"
#include "CloseDoor.h"
#include "FlashlightBreakdown.h"
#include "PlayFootstepSound.h"
#include "GhostAIController.h"


AGhostBase::AGhostBase()
{
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UGhostStatsComponent>(TEXT("GhostStats"));
}


// Overrided //
void AGhostBase::BeginPlay()
{
	Super::BeginPlay();

	InitGhostStats();

	GetMesh()->SetHiddenInGame(false);

	// AI Controller
	GhostAIController = Cast<AGhostAIController>(GetController());

	// Add Common behaviors
	CommonBehaviors.Add(NewObject<UPlayCommonSound>(this));
	CommonBehaviors.Add(NewObject<UManifest>(this));
	CommonBehaviors.Add(NewObject<UCloseDoor>(this));
	CommonBehaviors.Add(NewObject<UFlashlightBreakdown>(this));
	CommonBehaviors.Add(NewObject<UPlayFootstepSound>(this));
}

void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckBehaviorEventCondition();
}


// Ghost stats //
void AGhostBase::InitGhostStats()
{
	GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetMoveSpeed();
	PlayerDetectionRadius = StatsComp->GetPlayerDetectionRadius();
	SoundDetectionRadius = StatsComp->GetSoundDetectionRadius();
	BehaviorFrequency = StatsComp->GetBehaviorFrequency();

	BehaviorEventCycleDelay = 3.0f;
	bCanSetBehaviorEventCycleTimer = true;
	CurrentPatrolIndex = 0;
}

// Behavior event //
void AGhostBase::CheckBehaviorEventCondition()
{
	if (GhostAIController == nullptr) return;

	if (GhostAIController->GetPlayerSanityCorruptionRate() >= 10 && bCanSetBehaviorEventCycleTimer) {
		GetWorldTimerManager().SetTimer(GhostBehaviorCycleHandle, this, &AGhostBase::PerformBehaviorEvent, BehaviorEventCycleDelay, true);
		bCanSetBehaviorEventCycleTimer = false;
	}
	else if (GhostAIController->GetPlayerSanityCorruptionRate() < 10 && !bCanSetBehaviorEventCycleTimer) {
		GetWorldTimerManager().ClearTimer(GhostBehaviorCycleHandle);
		bCanSetBehaviorEventCycleTimer = true;
	}
	else return;
}

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
		
		RandEvidenceBehaviorNum = FMath::RandRange(0, EvidenceBehaviors.Num() - 1);
		if (EvidenceBehaviors[RandEvidenceBehaviorNum] == nullptr) return;
		EvidenceBehaviors[RandEvidenceBehaviorNum]->ExecuteBehavior(this);
	}
	else if (4 <= RandEventNum && RandEventNum <= 5)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Perform Common Behavior]"));
		
		RandCommonBehaviorNum = FMath::RandRange(0, CommonBehaviors.Num() - 1);
		if (CommonBehaviors[RandCommonBehaviorNum] == nullptr) return;
		CommonBehaviors[RandCommonBehaviorNum]->ExecuteBehavior(this);
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