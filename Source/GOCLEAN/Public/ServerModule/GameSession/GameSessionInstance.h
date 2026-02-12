// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameSessionInstance.generated.h"

/**
 * 
 */
UCLASS()
class GOCLEAN_API UGameSessionInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	/*virtual void Init() override;
	virtual void Shutdown() override;*/

private:
	// 현재 음성 채널 접속 여부
	bool bIsVoiceConnected = false;

	// 현재 생 음성 에너지 (0~1)
	float RawVoiceEnergy = 0.f;

	// 보정된 음성 레벨 (AI용)
	float CorrectedVoiceLevel = 0.f;

	// 마이크 상태
	bool bIsMicEnabled = true;

};
