// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerModule/GameSession/GameSessionMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

#include "ServerModule/GameSession/GameSessionState.h"
#include "ServerModule/GameSession/PlayerSessionState.h"
#include "Net/UnrealNetwork.h"


AGameSessionMode::AGameSessionMode()
{
	GameStateClass = AGameSessionState::StaticClass();
	PlayerStateClass = APlayerSessionState::StaticClass();
}


int32 AGameSessionMode::FindNextAvailableSeatIndex() const
{
	const AGameStateBase* GS = GameState;
	if (!GS) return 0;
	
	return GS->PlayerArray.Num();
}

int32 AGameSessionMode::GetCurrentPlayerCount() const
{
    TSet<int32> Used;

    const AGameStateBase* GS = GameState;
    if (GS)
    {
        for (APlayerState* PS : GS->PlayerArray)
        {
            const APlayerSessionState* PSS = Cast<APlayerSessionState>(PS);
            if (PSS)
            {
                Used.Add(PSS->GetSeatIndex());
            }
        }
    }

    for (int32 i = 0; i < MaxPlayers; ++i)
    {
        if (!Used.Contains(i))
            return i;
    }
    return INDEX_NONE;
}

void AGameSessionMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer || !GameState)
        return;

    // 인원수 넘어가면 kick (나중에 PreLogin으로 옮기기)
    const int32 CurrentCount = GetCurrentPlayerCount();
    if (CurrentCount > MaxPlayers)
    {
        const FText Reason = FText::FromString(TEXT("방이 가득 찼습니다."));
        
         // 최소한 클라를 메인메뉴로 돌려보내고 연결을 끊는 처리
         NewPlayer->ClientReturnToMainMenuWithTextReason(Reason);
         NewPlayer->Destroy();
        
        return;
    }

    // 참여 Index 꽉 찬 경우 Kick
    APlayerSessionState* PSS = Cast<APlayerSessionState>(NewPlayer->PlayerState);
    if (!PSS)
        return;

    

    const int32 Seat = FindNextAvailableSeatIndex();
    if (Seat == INDEX_NONE)
    {
        const FText Reason = FText::FromString(TEXT("좌석 할당 실패 (방이 가득 찼습니다)."));

        NewPlayer->ClientReturnToMainMenuWithTextReason(Reason);
        NewPlayer->Destroy();
        
        return;
    }

    PSS->SetSeatIndex(Seat);
    PSS->SetReady(false);
    PSS->SetEliminated(false);
    PSS->SetNickname(TEXT("Player"));

    // Online Subsystem Steam에서 닉네임 가져오기 로직 추가

    // 호스트는 자동으로 Ready 상태
    if (NewPlayer && NewPlayer->IsLocalController())
    {
        PSS->SetReady(true);
    }
   

    OnPlayerReadyChanged();

}

void AGameSessionMode::Logout(AController* Exiting)
{
    APlayerController* ExitingPC = Cast<APlayerController>(Exiting);
    APlayerSessionState* ExitingPSS = ExitingPC ? Cast<APlayerSessionState>(ExitingPC->PlayerState) : nullptr;

    const int32 LeavingSeat = ExitingPSS ? ExitingPSS->GetSeatIndex() : INDEX_NONE;

    // 호스트 여부
    const bool bIsHostLeaving = (ExitingPC && ExitingPC->IsLocalController());

    Super::Logout(Exiting);

    AGameStateBase* GS = GameState;
    AGameSessionState* SessionState = GetGameState<AGameSessionState>();


    // Index 당기기
    if (GS && LeavingSeat != INDEX_NONE)
    {
        for (APlayerState* PS : GS->PlayerArray)
        {
            APlayerSessionState* PSS = Cast<APlayerSessionState>(PS);
            if (!PSS) continue;

            if (PSS->GetSeatIndex() > LeavingSeat) 
            {
                PSS->SetSeatIndex(PSS->GetSeatIndex() - 1); 
            }
        }
    }

    // 생존자 수 갱신 (인게임 중 클라 강제 종료 시)
    if (SessionState && GS)
    {
        int32 AliveCount = 0;
        for (APlayerState* PS : GS->PlayerArray)
        {
            APlayerSessionState* PSS = Cast<APlayerSessionState>(PS);
            if (!PSS) continue;

            const bool bEliminated = PSS->IsEliminated();
            if (!bEliminated)
            {
                AliveCount++;
            }
        }

        SessionState->SetAliveSurvivorCount(AliveCount);

    }



    // 호스트가 떠날 경우 남은 플레이어 종료
    if (bIsHostLeaving)
    {
        const FText Reason = FText::FromString(TEXT("호스트가 나가서 세션이 종료되었습니다. 메인 메뉴로 돌아갑니다."));

        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (!PC) continue;

            if (PC->IsLocalController()) continue;

            PC->ClientReturnToMainMenuWithTextReason(Reason);
        }
    }

    OnPlayerReadyChanged();

    // Online Subsystem Steam 세션 종료 로직 추가
}

void AGameSessionMode::OnPlayerReadyChanged()
{
    if (!HasAuthority()) return;
    StartGameIfPossible();
}


bool AGameSessionMode::IsHostPlayerState(const APlayerState* PS) const
{
    if (!PS) return false;

    UWorld* World = GetWorld();
    if (!World) return false;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        const APlayerController* PC = It->Get();
        if (!PC) continue;

        if (PC->IsLocalController())
        {
            return (PC->PlayerState == PS);
        }
    }

    return false;
}


bool AGameSessionMode::CanStartGame() const
{
    if (!GameState) return false;

    int32 ActivePlayerCount = 0;
    int32 NonHostCount = 0;
    int32 NonHostReadyCount = 0;

    for (APlayerState* PS : GameState->PlayerArray)
    {
        APlayerSessionState* PSS = Cast<APlayerSessionState>(PS);
        if (!PSS) continue;


        ActivePlayerCount++;

        const bool bIsHost = IsHostPlayerState(PSS);
        if (bIsHost)
        {
            // 호스트는 Ready 체크에서 제외 (항상 Ready로 간주)
            continue;
        }

        NonHostCount++;
        if (PSS->IsReady())
        {
            NonHostReadyCount++;
        }
    }

    // 최소 인원 체크(호스트 포함)
    if (ActivePlayerCount < MinPlayersToStart)
        return false;

    // 호스트 제외 나머지 플레이어 레디 체크
    return (NonHostReadyCount == NonHostCount);
}

void AGameSessionMode::StartGameIfPossible()
{
    if(bGameStarted) return;

    if (CanStartGame())
    {
        bGameStarted = true;

        // 여기서 인게임 시작 처리

        UE_LOG(LogTemp, Log, TEXT("[GameSessionMode] StartGame triggered."));
    }
}