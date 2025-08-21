// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GhostStatsComponent.h"
#include "UEvidenceBehavior.h"
#include "CommonBehavior.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GhostBase.generated.h"


UCLASS(Abstract)
class GOCLEAN_API AGhostBase : public ACharacter
{
	GENERATED_BODY()

public:

	AGhostBase();

	// Using in GhostAIController.h/.cpp
	UPROPERTY(EditAnywhere, Category = "Patrol Points");
	TArray<TObjectPtr<AActor>> PatrolPoints;

	int32 CurrentPatrolIndex;

protected:

	// Behaviors //
	UPROPERTY(VisibleAnywhere, Category = "CommonBehaviors");
	TArray<TObjectPtr<UCommonBehavior>> UCommonBehaviors;

	UPROPERTY(VisibleAnywhere, Category = "EvidenceBehaviors");
	TArray<TObjectPtr<UEvidenceBehavior>> UEvidenceBehaviors;


	// Overrided //
	void BeginPlay() override;
	void Tick(float DeltaTime) override;


	// Ghost stats //
	void InitGhostStats();


	// Behavior event //
	void CheckBehaviorEventCondition();
	void PerformBehaviorEvent();





	// Ghost stats //
	float PlayerDetectionRadius;
	float SoundDetectionRadius;
	float BehaviorFrequency;


	// Player sanity //
	float SanityCorruptionRate;


	// Behavior event //
	float BehaviorEventCycleDelay;
	bool bCanSetBehaviorEventCycleTimer;
	FTimerHandle GhostBehaviorCycleHandle;

private:

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	TObjectPtr<UGhostStatsComponent> Stats;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComp;



};
