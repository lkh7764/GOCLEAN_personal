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

	Stats = CreateDefaultSubobject<UGhostStatsComponent>(TEXT("GhostStats"));
}


// Overrided //
void AGhostBase::BeginPlay()
{
	Super::BeginPlay();

	InitGhostStats();

	GetMesh()->SetHiddenInGame(false);

	// Add Common behaviors
	UCommonBehaviors.Add(NewObject<UPlayCommonSound>(this));
	UCommonBehaviors.Add(NewObject<UManifest>(this));
	UCommonBehaviors.Add(NewObject<UCloseDoor>(this));
	UCommonBehaviors.Add(NewObject<UFlashlightBreakdown>(this));
	UCommonBehaviors.Add(NewObject<UPlayFootstepSound>(this));
}

void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckBehaviorEventCondition();
}


// Ghost stats //
void AGhostBase::InitGhostStats()
{
	GetCharacterMovement()->MaxWalkSpeed = Stats->GetMoveSpeed();
	PlayerDetectionRadius = Stats->GetPlayerDetectionRadius();
	SoundDetectionRadius = Stats->GetSoundDetectionRadius();
	BehaviorFrequency = Stats->GetBehaviorFrequency();

	BehaviorEventCycleDelay = 3.0f;
	bCanSetBehaviorEventCycleTimer = true;
	CurrentPatrolIndex = 0;
}

// Behavior event //
void AGhostBase::CheckBehaviorEventCondition()
{
	AGhostAIController* GhostAIController = Cast<AGhostAIController>(GetController());
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