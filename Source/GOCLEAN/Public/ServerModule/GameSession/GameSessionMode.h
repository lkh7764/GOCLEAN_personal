// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameSessionMode.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API AGameSessionMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    AGameSessionMode();

protected:
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;
    virtual void BeginPlay() override;

public:
    // Ready가 바뀌었을 때 호출(서버)
    void OnPlayerReadyChanged();

    // 플레이어 로딩 완료 체크
    void OnPlayerLoadStateChanged();
protected:
    // 실제 게임 시작(맵 이동 등)
    void StartGameIfPossible();

    // 시작 가능 여부 판단
    bool CanStartGame() const;
    
    // 호스트 판별
    bool IsHostPlayerState(const APlayerState* PS) const;

    // 로딩 완료 체크/전환
    bool AreAllPlayersLoaded() const;
    void TransitionToLoadingPhase();
    void TransitionToInGameIfPossible();

public:
    // 로비에서 호스트가 의뢰(맵) 선택
    bool RequestSetContractId(int32 NewContractId);

    // 벤딩 구매 요청 (서버에서 규칙 검사 후 반영)
    bool RequestPurchaseVending(int32 BuyerPlayerId, int32 ItemId);

    // 플레이어 튕김/퇴장 시 구매 제거
    void HandlePlayerLeft(int32 PlayerId);

private:
    // 최대 동시 플레이어 수
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    int32 MaxPlayers = 4;

    // 시작을 위한 최소 레디 인원
    UPROPERTY(EditDefaultsOnly, Category = "Session")
    int32 MinPlayersToStart = 2; 

    // 게임 시작이 가능한지
    bool bGameStarted = false;

    // PlayerState의 PlayerArray를 통해 빈 Index 찾기
    int32 FindNextAvailableSeatIndex() const;

    // 접속 인원수 카운트
    int32 GetCurrentPlayerCount() const;

};
