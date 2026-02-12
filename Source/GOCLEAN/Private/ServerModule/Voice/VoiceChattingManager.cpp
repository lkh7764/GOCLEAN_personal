// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerModule/Voice/VoiceChattingManager.h"

#include "Modules/ModuleManager.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

#define VIVOX_VOICE_SERVER TEXT("https://unity.vivox.com/appconfig/14569-test-91617")
#define VIVOX_VOICE_DOMAIN TEXT("mtu1xp.vivox.com")
#define VIVOX_VOICE_ISSUER TEXT("14569-test-91617")
#define VIVOX_VOICE_KEY TEXT("KkLAV53qnqWCFd6xNEcHVnZnFMT53Txx")

void UVoiceChattingManager::InitVivox()
{
    if (bVivoxInitialized) return;

    auto* VivoxModule = static_cast<FVivoxCoreModule*>(
        &FModuleManager::Get().LoadModuleChecked(TEXT("VivoxCore"))
        );

    VivoxVoiceClient = &VivoxModule->VoiceClient();
    VivoxVoiceClient->Initialize();
    bVivoxInitialized = true;

    Vivox_ScreenLog(TEXT("[Vivox] Initialized"), FColor::Green);
}

// 디버그용. 나중에 삭제.
void UVoiceChattingManager::Vivox_ScreenLog(const FString& Msg, const FColor& Color, float Time)
{
    UE_LOG(LogTemp, Log, TEXT("%s"), *Msg);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
    }
}

void UVoiceChattingManager::Vivox_EnableEnergyMeter()
{
    ILoginSession& LoginSession = VivoxVoiceClient->GetLoginSession(LoggedInUserId);

    // Update10Hz / 5Hz / 1Hz 중 선택
    const VivoxCoreError Err = LoginSession.SetParticipantSpeakingUpdateRate(ParticipantSpeakingUpdateRate::Update10Hz);
    Vivox_ScreenLog(FString::Printf(TEXT("[Vivox] SetParticipantSpeakingUpdateRate Err=%d"), (int)Err), (Err == 0) ? FColor::Green : FColor::Red);
}

void UVoiceChattingManager::Vivox_TickDebugHud()
{
    if (!bDebugHudEnabled || !VivoxVoiceClient) return;

    // 로그인/채널 기본 상태 출력
    FString Base = FString::Printf(TEXT("[Vivox] Init=%d Joined=%d"), bVivoxInitialized ? 1 : 0, bHasJoinedChannel ? 1 : 0);

    if (!bHasJoinedChannel)
    {
        Vivox_ScreenLog(Base, FColor::Silver, 0.25f);
        return;
    }

    ILoginSession& LoginSession = VivoxVoiceClient->GetLoginSession(LoggedInUserId);
    IChannelSession& ChannelSession = LoginSession.GetChannelSession(JoinedChannel);

    const auto& Participants = ChannelSession.Participants();

    IParticipant* Self = nullptr;
    for (const auto& Kvp : Participants)
    {
        if (Kvp.Value && Kvp.Value->IsSelf())
        {
            Self = Kvp.Value;
            break;
        }
    }

    if (!Self)
    {
        Vivox_ScreenLog(Base + TEXT(" | Self=NONE"), FColor::Silver, 0.25f);
        return;
    }

    LastSelfAudioEnergy = Self->AudioEnergy();
    bLastSelfSpeechDetected = Self->SpeechDetected();

    const FString Meter = FString::Printf(TEXT("%s | SelfEnergy=%.3f Speech=%d InAudio=%d Tx=%d"),
        *Base,
        LastSelfAudioEnergy,
        bLastSelfSpeechDetected ? 1 : 0,
        Self->InAudio() ? 1 : 0,
        ChannelSession.IsTransmitting() ? 1 : 0);

    Vivox_ScreenLog(Meter, bLastSelfSpeechDetected ? FColor::Green : FColor::Silver, 0.25f);
}

void UVoiceChattingManager::Vivox_LoginSimple(const FString& PlayerIdOverride)
{
    if (!VivoxVoiceClient) InitVivox();

    FString PlayerId = PlayerIdOverride;
    if (PlayerId.IsEmpty())
    {
        PlayerId = FString::Printf(TEXT("Player_%lld"), FDateTime::UtcNow().GetTicks());
    }

    LoggedInUserId = AccountId(VIVOX_VOICE_ISSUER, PlayerId, VIVOX_VOICE_DOMAIN);
    ILoginSession& LoginSession = VivoxVoiceClient->GetLoginSession(LoggedInUserId);

    // 토큰 유효 시간 : 테스트용 (1시간)
    FTimespan TokenExpiration = FTimespan::FromHours(1);
    FString LoginToken = LoginSession.GetLoginToken(VIVOX_VOICE_KEY, TokenExpiration);

    ILoginSession::FOnBeginLoginCompletedDelegate OnLoginDone;
    OnLoginDone.BindLambda([this](VivoxCoreError Error)
        {
            const bool bOk = (Error == 0);
            Vivox_ScreenLog(
                FString::Printf(TEXT("[Vivox] Login %s (Error=%d)"),
                    bOk ? TEXT("SUCCESS") : TEXT("FAIL"), (int)Error),
                bOk ? FColor::Green : FColor::Red);

            if (bOk)
            {
                Vivox_EnableEnergyMeter();
            }
        });

    Vivox_ScreenLog(FString::Printf(TEXT("[Vivox] BeginLogin as '%s'"), *PlayerId), FColor::Cyan);
    LoginSession.BeginLogin(VIVOX_VOICE_SERVER, LoginToken, OnLoginDone);
}
