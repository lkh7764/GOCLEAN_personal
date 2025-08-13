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

protected:
	AGhostBase();

	// Stats component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UGhostStatsComponent* Stats;
	USkeletalMeshComponent* Mesh;

	TArray<UEvidenceBehavior*> UEvidenceBehaviors;
	TArray<UCommonBehavior*> UCommonBehaviors;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Stats
	float PlayerDetectionRadius;
	float SoundDetectionRadius;
	float BehaviorFrequency;

	// association variables with PlayerCharacter
	float SanityCorruptionRate;

	// Behavior Event Declaration
	FTimerHandle GhostBehaviorCycleHandle;
	float BehaviorEventCycleDelay;
	bool bCanSetTimer;

	void CheckBehaviorEventCondition();
	void PerformBehaviorEvent();

	void InitGhostStats();

public:
	UPROPERTY(EditAnywhere, Category = "Patrol Points");
	TArray<AActor*> PatrolPoints;
	int32 CurrentPatrolIndex;
};
