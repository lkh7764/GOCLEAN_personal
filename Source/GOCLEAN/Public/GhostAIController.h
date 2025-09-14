#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GhostBase.h"
#include "GhostAIController.generated.h"

UCLASS()
class GOCLEAN_API AGhostAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	// Getter //
	float GetPlayerSanityCorruptionRate() const;


private:

	// Overrieded //
	void BeginPlay() override; // JSH TMP
	void Tick(float DeltaTime) override;
	void OnPossess(APawn* InPawn) override;


	// Player sanity check //
	void CheckPlayerSanityCorruptionRate();


	// State //

	// Patrol
	void MoveToPatrolPoint();
	void CheckArrivalCurrentPatrolPoint();

	// Chase
	void CheckEnrageEventCondition();
	void StartChase();
	void ChasePlayer();

	// Hunt
	void PlayerHunt();




	// Check player sanity //
	TObjectPtr<AGOCLEANCharacter> Player;
	float PlayerSanityCorruptionRate;
	FTimerHandle CheckPlayerSanityCorruptionHandle;


	// State //

	// Patrol
	bool bIsPatrolling;
	FTimerHandle CheckArrivalToCurrentPointHandle;

	// Chase
	bool bIsChasing;
	bool bIsEnrageEvent;
	FTimerHandle ChasingPlayerHandle;

	// Hunt
	float ManifestRadius;
	float HuntRadius;

};