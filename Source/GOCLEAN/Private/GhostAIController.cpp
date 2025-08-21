#include "GhostAIController.h"
#include "GhostBase.h"
#include "GOCLEAN/GOCLEANCharacter.h"


// Getter //
float AGhostAIController::GetPlayerSanityCorruptionRate() const { return PlayerSanityCorruptionRate; };


// Overrided //
void AGhostAIController::BeginPlay() // JSH TMP
{
	Super::BeginPlay();
	MoveToPatrolPoint();
}

void AGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!bIsEnrageEvent) CheckEnrageEventCondition();
}

void AGhostAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorld()->GetTimerManager().SetTimer(CheckPlayerSanityCorruptionHandle, this, &AGhostAIController::CheckPlayerSanityCorruptionRate, 5.0f, true);

	bIsEnrageEvent = false;
	bIsChasing = false;
	bIsPatrolling = true;

	ManifestRadius = 500.0f;
	HuntRadius = 100.0f;

	//MoveToPatrolPoint();
}


// Check player sanity //
void AGhostAIController::CheckPlayerSanityCorruptionRate()
{
	UE_LOG(LogTemp, Warning, TEXT("Ghost Get player sanity"));
	AGOCLEANCharacter* Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr) return;

	// JSH Flag: Sanity
	UE_LOG(LogTemp, Warning, TEXT("Player's current sanity corruption rate: %f"), (100.0f - Player->GetCurrentSanity()));

	PlayerSanityCorruptionRate = (100.0f - Player->GetCurrentSanity());
}


// State //
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

	GetWorld()->GetTimerManager().SetTimer(CheckArrivalCurrentPointHandle, this, &AGhostAIController::CheckArrivalCurrentPatrolPoint, 1.0f, true);
}

void AGhostAIController::CheckArrivalCurrentPatrolPoint()
{
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr || GhostCharacter->PatrolPoints.Num() == 0) return;

	AActor* TargetPoint = GhostCharacter->PatrolPoints[GhostCharacter->CurrentPatrolIndex];
	if (TargetPoint == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPoint->GetActorLocation());
	if (Distance < 50.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CheckArrivalCurrentPointHandle);

		GhostCharacter->CurrentPatrolIndex = (GhostCharacter->CurrentPatrolIndex + 1) % (GhostCharacter->PatrolPoints.Num());
		MoveToPatrolPoint();
	}
}

void AGhostAIController::CheckEnrageEventCondition()
{
	if (GetPlayerSanityCorruptionRate() >= 30 && !bIsEnrageEvent) {
		bIsEnrageEvent = true;
		StartChase();
	}
	else return;
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

	// JSH TODO: With Server
	AActor* TargetPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TargetPlayerCharacter == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPlayerCharacter->GetActorLocation());
	if (Distance < ManifestRadius)
	{
		GhostCharacter->GetMesh()->SetHiddenInGame(false);
	}

	if (Distance < HuntRadius)
	{
		GhostCharacter->GetMesh()->SetHiddenInGame(true);

		UE_LOG(LogTemp, Warning, TEXT("Player Hunted"));

		GetWorld()->GetTimerManager().ClearTimer(ChasingPlayerHandle);
		bIsEnrageEvent = false;
		bIsChasing = false;
		bIsPatrolling = true;
		PlayerSanityCorruptionRate = 0;

		// JSH Temp: Player sanity reset
		AGOCLEANCharacter* Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player == nullptr) return;
		Player->SetCurrentSanity(100.0f);
		MoveToPatrolPoint();
	}
}