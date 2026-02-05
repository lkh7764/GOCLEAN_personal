/**
 * @class AGhostAIController
 * @brief Ghost actor의 상태 제어 및, 플레이어 정신력에 따른 행동 변화를 관리하는 AI Controller
 * 
 * [Core functions]
 * - CheckPlayerSanityCorruptionRate(): 플레이어들의 정신력을 기반으로, 정신 오염도 계산
 * - MoveToPatrolPoint(): Patrol point로의 이동
 * - CheckArrivalCurrentPatrolPoint(): 현재 대상 Patrol point에 도착했는지를 체크
 * - CheckEnrageEventCondition(): 격노 이벤트 조건 확인
 * - StartChase(): 격노 상태로의 전환
 * - ChasePlayer(): 격노 이벤트 로직 수행
 * - PlayerHunt(): 플레이어 Hunt 처리
 */

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GEnemy/Base/GhostBase.h"
#include "GhostAIController.generated.h"

class AGOCLEANCharacter;

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