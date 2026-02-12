// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VivoxCore.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VoiceChattingManager.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API UVoiceChattingManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:


private:
    void InitVivox();
    bool bVivoxInitialized = false;

    //IClient* VivoxVoiceClient = nullptr;
    //AccountId LoggedInUserId;

    //// 현재 조인한 채널 추적용
    //ChannelId JoinedChannel;
    //bool bHasJoinedChannel = false;
	
};
