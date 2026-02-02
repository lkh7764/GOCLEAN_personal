// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerModule/GameSession/PlayerSessionState.h"
#include <Net/UnrealNetwork.h>

APlayerSessionState::APlayerSessionState()
{
	bReplicates = true;
}

void APlayerSessionState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerSessionState, SeatIndex);
	DOREPLIFETIME(APlayerSessionState, bReady);
}

void APlayerSessionState::OnRep_SeatIndex()
{
	// UI 갱신
}

void APlayerSessionState::OnRep_Ready()
{
	// UI 갱신
}
