// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GhostStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UGhostStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class AGhostBase;


public:

	UGhostStatsComponent();



	// Init
	void InitializeGhostStats(
		float BaseMoveSpeed,
		float BasePlayerDetectionRadius,
		float BaseSoundDetectionRadius,
		float BaseBehaviourFrequency)
	{
		MoveSpeed = BaseMoveSpeed;
		PlayerDetectionRadius = BasePlayerDetectionRadius;
		SoundDetectionRadius = BaseSoundDetectionRadius;
		BehaviourFrequency = BaseBehaviourFrequency;
	}

private:

	//JSH Temp
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float PlayerDetectionRadius;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float SoundDetectionRadius;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float BehaviourFrequency;



	
	float GetMoveSpeed() const;
	float GetPlayerDetectionRadius() const;
	float GetSoundDetectionRadius() const;
	float GetBehaviorFrequency() const;

};