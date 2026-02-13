#include "GEnemy/GhostAIController.h"
#include "GEnemy/Base/GhostBase.h"
#include "GCharacter/GOCLEANCharacter.h"


// Getter //
float AGhostAIController::GetPlayerSanityCorruptionRate() const { return PlayerSanityCorruptionRate; };


// Overrided //
void AGhostAIController::BeginPlay()
{
	Super::BeginPlay();

	// JSH TMP
	Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MoveToPatrolPoint();
}

void AGhostAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!bIsRageEvent) 
		CheckRageEventCondition();
}

void AGhostAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorld()->GetTimerManager().SetTimer(CheckPlayerSanityCorruptionHandle, this, &AGhostAIController::CheckPlayerSanityCorruptionRate, 5.0f, true);

	bIsRageEvent = false;
	bIsChasing = false;
	bIsPatrolling = true;

	ManifestRadius = 300.0f;
	HuntRadius = 100.0f;

	//MoveToPatrolPoint();
}


// Check player sanity //
void AGhostAIController::CheckPlayerSanityCorruptionRate()
{
	UE_LOG(LogTemp, Warning, TEXT("Ghost Get player sanity"));
	if (Player == nullptr) return;

	// JSH Flag: Sanity
	UE_LOG(LogTemp, Warning, TEXT("Player's current sanity corruption rate: %f"), (100.0f - Player->GetPlayerCurrentSanity()));

	PlayerSanityCorruptionRate = (100.0f - Player->GetPlayerCurrentSanity());
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

	GetWorld()->GetTimerManager().SetTimer(CheckArrivalToCurrentPointHandle, this, &AGhostAIController::CheckArrivalCurrentPatrolPoint, 1.0f, true);
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
		GetWorld()->GetTimerManager().ClearTimer(CheckArrivalToCurrentPointHandle);

		GhostCharacter->CurrentPatrolIndex = (GhostCharacter->CurrentPatrolIndex + 1) % (GhostCharacter->PatrolPoints.Num());
		MoveToPatrolPoint();
	}
}

void AGhostAIController::CheckRageEventCondition()
{
	if (GetPlayerSanityCorruptionRate() >= 30 && !bIsRageEvent) {
		bIsRageEvent = true;
		StartChase();
	}
	else return;
}

void AGhostAIController::StartUnendingRageEvent()
{
	bIsUnendingRageEvent = true;
	StartChase();
}

void AGhostAIController::StartChase()
{
	bIsChasing = true;
	bIsPatrolling = false;

	ChasePlayer(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AGhostAIController::ChasePlayer(AActor* TargetPlayerCharacter)
{
	if (TargetPlayerCharacter == nullptr) return;

	MoveToActor(TargetPlayerCharacter);

	if(bIsRageEvent) GetWorld()->GetTimerManager().SetTimer(ChasingPlayerHandle, this, &AGhostAIController::PlayerHunt, 0.2f, true);
	else if(bIsUnendingRageEvent) GetWorld()->GetTimerManager().SetTimer(ChasingPlayerHandle, this, &AGhostAIController::EndlessPlayerHunt, 0.2f, true);
}

void AGhostAIController::PlayerHunt()
{
	if (!bIsRageEvent) return;

	//UE_LOG(LogTemp, Warning, TEXT("Hunting Player"));
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr) return;

	// JSH TODO: With Server
	AActor* TargetPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TargetPlayerCharacter == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPlayerCharacter->GetActorLocation());
	if (Distance < ManifestRadius)
	{
		GhostCharacter->GetMesh()->SetHiddenInGame(true);
	}

	if (Distance < HuntRadius)
	{
		GhostCharacter->GetMesh()->SetHiddenInGame(false);

		Cast<AGOCLEANCharacter>(TargetPlayerCharacter)->OnHunted();

		UE_LOG(LogTemp, Warning, TEXT("Player Hunted"));

		GetWorld()->GetTimerManager().ClearTimer(ChasingPlayerHandle);
		bIsRageEvent = false;
		bIsChasing = false;
		bIsPatrolling = true;
		PlayerSanityCorruptionRate = 0;

		// JSH Temp: Player sanity reset
		//AGOCLEANCharacter* Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player == nullptr) return;
		Player->SetPlayerCurrentSanity(100.0f);

		MoveToPatrolPoint();
	}
}

void AGhostAIController::EndlessPlayerHunt()
{
	if (!bIsUnendingRageEvent) return;

	//UE_LOG(LogTemp, Warning, TEXT("Hunting Player"));
	AGhostBase* GhostCharacter = Cast<AGhostBase>(GetPawn());
	if (GhostCharacter == nullptr) return;

	// JSH TODO: With Server
	AActor* TargetPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (TargetPlayerCharacter == nullptr) return;

	float Distance = FVector::Dist(GhostCharacter->GetActorLocation(), TargetPlayerCharacter->GetActorLocation());
	if (Distance < ManifestRadius)
	{
		GhostCharacter->GetMesh()->SetHiddenInGame(true);
	}

	if (Distance < HuntRadius)
	{
		GhostCharacter->GetMesh()->SetHiddenInGame(false);

		Cast<AGOCLEANCharacter>(TargetPlayerCharacter)->OnHunted();

		UE_LOG(LogTemp, Warning, TEXT("Player Hunted"));

		GetWorld()->GetTimerManager().ClearTimer(ChasingPlayerHandle);
		bIsChasing = false;
		bIsPatrolling = true;

		// JSH Temp: Player sanity reset
		//AGOCLEANCharacter* Player = Cast<AGOCLEANCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (bIsUnendingRageEvent) EndlessPlayerHunt();
	}
}