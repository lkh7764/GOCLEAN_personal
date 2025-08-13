#include "GhostAIController.h"
#include "GhostBase.h"
#include "GOCLEAN/GOCLEANCharacter.h"

void AGhostAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bIsEnrageEvent = false;
	bIsChasing = false;
	bIsPatrolling = true;
}

void AGhostAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveToPatrolPoint();
	GetWorld()->GetTimerManager().SetTimer(CheckPlayerSanityCorruptionHandle, this, &AGhostAIController::CheckPlayerSanityCorruptionRate, 5.0f, true);
}

void AGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsEnrageEvent) CheckEnrageEventCondition();
}

void AGhostAIController::CheckPlayerSanityCorruptionRate()
{
	UE_LOG(LogTemp, Warning, TEXT("Ghost Get player sanity"));
	AGOCLEANCharacter* Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) return;

	// JSH Flag: Sanity
	UE_LOG(LogTemp, Warning, TEXT("Player's current sanity corruption rate: %f"), (100.0f - Player->GetCurrentSanity()));

	_PlayerSanityCorruptionRate = (100.0f - Player->GetCurrentSanity());
}

float AGhostAIController::GetPlayerSanityCorruptionRate() const { return _PlayerSanityCorruptionRate; };

void AGhostAIController::CheckEnrageEventCondition()
{
	if (GetPlayerSanityCorruptionRate() >= 30 && !bIsEnrageEvent) {
		bIsEnrageEvent = true;
		StartChase();
	}
	else return;
}

void AGhostAIController::MoveToPatrolPoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Move to next patrol point"));
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr || GhostCharacter->PatrolPoints.Num() == 0) return;

	AActor* TargetPoint = GhostCharacter->PatrolPoints[GhostCharacter->CurrentPatrolIndex];
	if (TargetPoint == nullptr) return;

	if (bIsPatrolling == true && bIsChasing == false) {
		MoveToActor(TargetPoint);
	}

	GetWorld()->GetTimerManager().SetTimer(CheckArrivalHandle, this, &AGhostAIController::CheckArrival, 1.0f, true);
}

// Chase 추가 시 bool 판단
void AGhostAIController::CheckArrival()
{
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr || GhostCharacter->PatrolPoints.Num() == 0) return;

	AActor* TargetPoint = GhostCharacter->PatrolPoints[GhostCharacter->CurrentPatrolIndex];
	if (TargetPoint == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPoint->GetActorLocation());
	if (Distance < 50.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckArrivalHandle);

		GhostCharacter->CurrentPatrolIndex = (GhostCharacter->CurrentPatrolIndex + 1) % (GhostCharacter->PatrolPoints.Num());
		MoveToPatrolPoint();
	}
}

void AGhostAIController::StartChase()
{
	bIsChasing = true;
	bIsPatrolling = false;

	ChasePlayer();
}

void AGhostAIController::ChasePlayer()
{
	AActor* TargetPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TargetPlayerCharacter == nullptr) return;

	MoveToActor(TargetPlayerCharacter);

	GetWorld()->GetTimerManager().SetTimer(ChasingPlayerHandle, this, &AGhostAIController::PlayerHunt, 0.2f, true);
}

void AGhostAIController::PlayerHunt()
{
	if (!bIsEnrageEvent) return;

	//UE_LOG(LogTemp, Warning, TEXT("Hunting Player"));
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr) return;

	AActor* TargetPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TargetPlayerCharacter == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPlayerCharacter->GetActorLocation());
	if (Distance < 100.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Hunted"));

		GetWorld()->GetTimerManager().ClearTimer(ChasingPlayerHandle);
		bIsEnrageEvent = false;
		bIsChasing = false;
		bIsPatrolling = true;

		// JSH Temp: Player sanity reset
		AGOCLEANCharacter* Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player == nullptr) return;
		Player->IncreaseSanity(10.0f);

		MoveToPatrolPoint();
	}
}