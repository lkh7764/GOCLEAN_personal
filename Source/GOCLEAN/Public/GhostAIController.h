#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostBase.h"
#include "GhostAIController.generated.h"

UCLASS()
class GOCLEAN_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void OnPossess(APawn* InPawn) override;

	// Patrol event
	bool bIsPatrolling;
	FTimerHandle CheckArrivalHandle;

	void MoveToPatrolPoint();
	void CheckArrival();

	// Enrage event
	bool bIsChasing;
	bool bIsEnrageEvent;

	void CheckEnrageEventCondition();
	void StartChase();
	void ChasePlayer();
	void PlayerHunt();

	FTimerHandle ChasingPlayerHandle;

	// Sanity check
	float _PlayerSanityCorruptionRate;
	FTimerHandle CheckPlayerSanityCorruptionHandle;

	void CheckPlayerSanityCorruptionRate();
public:
	float GetPlayerSanityCorruptionRate() const;
};