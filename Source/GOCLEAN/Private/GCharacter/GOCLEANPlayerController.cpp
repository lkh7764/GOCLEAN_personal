#include "GCharacter/GOCLEANPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

#include "Net/RpcTypes.h"
#include "ServerModule/GameSession/RPCRouterComponent.h"

AGOCLEANPlayerController::AGOCLEANPlayerController()
{
	RPCRouter = CreateDefaultSubobject<URPCRouterComponent>(TEXT("RPCRouter"));
}


void AGOCLEANPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AGOCLEANPlayerController::ShowTitleUI()
{
    // 기존 위젯 제거
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (!TitleWidgetClass) return;

    CurrentWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass);
    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport();

        bShowMouseCursor = true;
        FInputModeUIOnly Mode;
        SetInputMode(Mode);
    }
}

void AGOCLEANPlayerController::ShowLobbyUI()
{

    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (!LobbyWidgetClass) return;

    CurrentWidget = CreateWidget<UUserWidget>(this, LobbyWidgetClass);
    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport();

        bShowMouseCursor = true;
        FInputModeUIOnly Mode;
        SetInputMode(Mode);
    }
}

void AGOCLEANPlayerController::ShowResultUI()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (!ResultWidgetClass) return;

    CurrentWidget = CreateWidget<UUserWidget>(this, ResultWidgetClass);
    if (CurrentWidget)
    {
        CurrentWidget->AddToViewport();

        bShowMouseCursor = true;
        FInputModeUIOnly Mode;
        SetInputMode(Mode);
    }
}


void AGOCLEANPlayerController::Client_ShowResultUI_Implementation()
{
    ShowResultUI();
}


void AGOCLEANPlayerController::TryDoInteraction()
{
    FObjectPayload_C2S Temp;
    RPCRouter->Server_ObjectEvent(EObjectEvent_C2S::Object_TryInteract, Temp);
}

void AGOCLEANPlayerController::ChangeSlot(int32 SlotIndex)
{
    FPlayerPayload_C2S Temp;
    Temp.ParamInt = SlotIndex;

    RPCRouter->Server_PlayerEvent(EPlayerEvent_C2S::RequestChangeCurrentSlotIndex, Temp);
}
