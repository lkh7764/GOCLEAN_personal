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
    UFUNCTION(BlueprintCallable, Category = "Vivox")
    void Vivox_LoginSimple(const FString& PlayerIdOverride);

   /* UFUNCTION(BlueprintCallable, Category = "Vivox")
    void Vivox_JoinVoice(const FString& ChannelName, bool bPositional);

    UFUNCTION(BlueprintCallable, Category = "Vivox")
    void Vivox_LeaveVoice();

    UFUNCTION(BlueprintCallable, Category = "Vivox")
    void Vivox_StartVoice(const FString& PlayerIdOverride, const FString& ChannelName, bool bPositional);

    UFUNCTION(BlueprintCallable, Category = "Vivox")
    void Vivox_SetMicActive(bool bMuted);

public: 
    UFUNCTION(BlueprintCallable, Category = "Vivox|3D")
    void Vivox_Update3DFromActors(AActor* SpeakerActor, AActor* ListenerActor);

    UFUNCTION(BlueprintCallable, Category = "Vivox|3D")
    void Vivox_Start3DUpdate(AActor* SpeakerActor, AActor* ListenerActor, float Interval = 0.1f);

    UFUNCTION(BlueprintCallable, Category = "Vivox|3D")
    void Vivox_Stop3DUpdate();

    UFUNCTION(BlueprintCallable, Category = "Vivox|Debug")
    void Vivox_SetDebugHudEnabled(bool bEnabled);*/

private:
    void InitVivox();
    bool bVivoxInitialized = false;

    IClient* VivoxVoiceClient = nullptr;
    AccountId LoggedInUserId;

    // 현재 조인한 채널 추적용
    ChannelId JoinedChannel;
    bool bHasJoinedChannel = false;

private:
    // 로그인/조인 요청을 묶기 위한 내부 상태
    bool bLoginInProgress = false;
    bool bIsLoggedIn = false;

    FString PendingPlayerId;
    FString PendingChannelName;
    bool bPendingPositional = false;

    //// 기존 Vivox_LoginSimple을 그대로 써도 되지만, StartVoice에서만 쓰는 용
    //void Vivox_BeginLoginInternal(const FString& PlayerIdOverride);

    //void Vivox_TryAutoStart3DUpdate();
    //void Vivox_Update3D_Internal();

    TWeakObjectPtr<AActor> CachedSpeaker;
    TWeakObjectPtr<AActor> CachedListener;

    FTimerHandle Vivox3DTimer;

    bool bAutoStart3DUpdate = true;
    float Auto3DUpdateInterval = 0.1f;

private:
    // 여기는 디버그용

    // 화면 로그 헬퍼
    void Vivox_ScreenLog(const FString& Msg, const FColor& Color = FColor::Green, float Time = 6.f);

    // 음성 에너지(AuidoEnergy) 갱신용
    void Vivox_EnableEnergyMeter();      // SetParticipantSpeakingUpdateRate 설정
    void Vivox_TickDebugHud();           // Timer로 주기 출력

    // Debug HUD
    bool bDebugHudEnabled = true;
    FTimerHandle DebugHudTimerHandle;

    // 마지막 측정값
    double LastSelfAudioEnergy = 0.0;
    bool bLastSelfSpeechDetected = false;
	
};
