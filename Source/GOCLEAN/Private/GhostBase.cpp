#include "GhostBase.h"

// Sets default values
AGhostBase::AGhostBase()
{
	PrimaryActorTick.bCanEverTick = true;
	// Tick(float DeltaTime) 사용 가능

	// Stats Component
	Stats = CreateDefaultSubobject<UGhostStatsComponent>(TEXT("GhostStats"));
}

void AGhostBase::BeginPlay()
{
	Super::BeginPlay();
	//나중에 자식 객체에다 옮기면 될 듯
	GetCharacterMovement()->MaxWalkSpeed = Stats->GetMoveSpeed();
}

void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EventPerform();
}

void AGhostBase::StartEnragedChase(AActor* Target)
{
	bIsEnraged = true;
	TargetActor = Target;
}


void AGhostBase::EventPerform()
{
	int32 RandEventNum;
	RandEventNum = FMath::RandRange(0, 8);

	if (0 <= RandEventNum && RandEventNum <= 3)
	{
		if (UEvidenceBehaviors.Num() <= 0) return;
		int32 RandEvidenceBehaviorNum;
		RandEvidenceBehaviorNum = FMath::RandRange(0, UEvidenceBehaviors.Num() - 1);
		UEvidenceBehaviors[RandEvidenceBehaviorNum]->ExecuteBehavior(this);
	}
}


/*
void AGhostBase::PlayCommonSound() {
	if (CommonSound == nullptr) return;

	UGameplayStatics::PlaySoundAtLocation(this, CommonSound, GetActorLocation());
}
void AGhostBase::Manifest()
{
	GetMesh()->SetVisibility(true);
}

void AGhostBase::Patrol()
{
	// Patrol
}
void AGhostBase::CloseDoor()
{
	// function
}
void AGhostBase::PlayFootstepSound()
{
	// function
}
*/