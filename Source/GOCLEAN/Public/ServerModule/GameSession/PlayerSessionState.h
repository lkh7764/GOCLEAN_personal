// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerSessionState.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API APlayerSessionState : public APlayerState
{
	GENERATED_BODY()

public:
	APlayerSessionState();

	// 로비 //

	// 세션 참여 순서
	UPROPERTY(ReplicatedUsing = OnRep_SeatIndex, BlueprintReadOnly)
	int32 SeatIndex = -1;

	// 준비 상태
	UPROPERTY(ReplicatedUsing = OnRep_Ready, BlueprintReadOnly)
	bool bReady = false;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 로비 //

	UFUNCTION()
	void OnRep_SeatIndex();

	UFUNCTION()
	void OnRep_Ready();
	
};
