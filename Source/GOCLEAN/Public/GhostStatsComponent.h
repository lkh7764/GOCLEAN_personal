// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GhostStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UGhostStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGhostStatsComponent();
	int GetMoveSpeed();

protected:
	UPROPERTY(EditAnywhere, Category="Stats")
	float MoveSpeed;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float PlayerDetectionRadius;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float SoundDetectionRadius;
	//각 게임마다 랜덤하게 할당되는 값 (상수 a~b사이의 값)
	UPROPERTY(EditAnywhere, Category = "Stats")
	int BehaviorFrequency;
};