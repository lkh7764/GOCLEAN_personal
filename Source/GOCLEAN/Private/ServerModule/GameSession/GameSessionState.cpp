// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerModule/GameSession/GameSessionState.h"
#include <Net/UnrealNetwork.h>

AGameSessionState::AGameSessionState()
{
    ObjectManager = nullptr;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
            FString::Printf(TEXT("[AGameSessionState] AGameSessionState()")));
    }


	bReplicates = true;
}

void AGameSessionState::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        ObjectManager = NewObject<UGObjectManager>(this);
    }
}

void AGameSessionState::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    
    if (HasAuthority() && !ObjectManager)
    {
        ObjectManager = NewObject<UGObjectManager>(this);
    }
}

void AGameSessionState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AGameSessionState, SpiritualGauge);
    DOREPLIFETIME(AGameSessionState, RestGauge);
    DOREPLIFETIME(AGameSessionState, ExorcismProgress);
    DOREPLIFETIME(AGameSessionState, PostExorcismTimeRemaining);
    DOREPLIFETIME(AGameSessionState, AliveSurvivorCount);
    DOREPLIFETIME(AGameSessionState, FinalRewardMoney);
}

static float ClampFloat(float Value, float Min, float Max)
{
    return FMath::Clamp(Value, Min, Max);
}

void AGameSessionState::AddSpiritualGauge(float Delta, float Min, float Max)
{
    if (!HasAuthority()) return;
    SetSpiritualGauge_Internal(ClampFloat(SpiritualGauge + Delta, Min, Max));
}

void AGameSessionState::AddRestGauge(float Delta, float Min, float Max)
{
    if (!HasAuthority()) return;
    SetRestGauge_Internal(ClampFloat(RestGauge + Delta, Min, Max));
}

void AGameSessionState::SetExorcismProgress(float NewProgress, float Min, float Max)
{
    if (!HasAuthority()) return;
    SetExorcismProgress_Internal(ClampFloat(NewProgress, Min, Max));
}

void AGameSessionState::AddExorcismProgress(float Delta, float Min, float Max)
{
    if (!HasAuthority()) return;
    SetExorcismProgress_Internal(ClampFloat(ExorcismProgress + Delta, Min, Max));
}

void AGameSessionState::StartPostExorcismTimer(float DurationSeconds)
{
    if (!HasAuthority()) return;
    SetPostExorcismTimeRemaining_Internal(FMath::Max(0.f, DurationSeconds));
}

void AGameSessionState::SetPostExorcismTimeRemaining(float NewRemainingSeconds)
{
    if (!HasAuthority()) return;
    SetPostExorcismTimeRemaining_Internal(FMath::Max(0.f, NewRemainingSeconds));
}

void AGameSessionState::AddPostExorcismTimeRemaining(float DeltaSeconds)
{
    if (!HasAuthority()) return;
    SetPostExorcismTimeRemaining_Internal(FMath::Max(0.f, PostExorcismTimeRemaining + DeltaSeconds));
}

void AGameSessionState::SetAliveSurvivorCount(int32 NewCount)
{
    if (!HasAuthority()) return;
    SetAliveSurvivorCount_Internal(FMath::Max(0, NewCount));
}

void AGameSessionState::AddAliveSurvivorCount(int32 Delta)
{
    if (!HasAuthority()) return;
    SetAliveSurvivorCount_Internal(FMath::Max(0, AliveSurvivorCount + Delta));
}

void AGameSessionState::SetFinalRewardMoney(int32 NewMoney)
{
    if (!HasAuthority()) return;
    SetFinalRewardMoney_Internal(FMath::Max(0, NewMoney));
}



// Internal setters (server-only)
void AGameSessionState::SetSpiritualGauge_Internal(float NewValue)
{
    SpiritualGauge = NewValue;
    
    OnRep_SpiritualGauge();
}

void AGameSessionState::SetRestGauge_Internal(float NewValue)
{
    RestGauge = NewValue;

    OnRep_RestGauge();
}

void AGameSessionState::SetExorcismProgress_Internal(float NewValue)
{
    ExorcismProgress = NewValue;

    OnRep_ExorcismProgress();
}

void AGameSessionState::SetPostExorcismTimeRemaining_Internal(float NewValue)
{
    PostExorcismTimeRemaining = NewValue;

    OnRep_PostExorcismTimeRemaining();
}

void AGameSessionState::SetAliveSurvivorCount_Internal(int32 NewValue)
{
    AliveSurvivorCount = NewValue;

    OnRep_AliveSurvivorCount();
}

void AGameSessionState::SetFinalRewardMoney_Internal(int32 NewValue)
{
    FinalRewardMoney = NewValue;

    OnRep_FinalRewardMoney();
}



// OnRep (Client UI 갱신)
void AGameSessionState::OnRep_SpiritualGauge()
{

}

void AGameSessionState::OnRep_RestGauge()
{

}

void AGameSessionState::OnRep_ExorcismProgress()
{

}

void AGameSessionState::OnRep_PostExorcismTimeRemaining()
{

}

void AGameSessionState::OnRep_AliveSurvivorCount()
{

}

void AGameSessionState::OnRep_FinalRewardMoney()
{
    // UI 갱신(결과창)
}