#include "GCharacter/GOCLEANPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

#include "Net/RpcTypes.h"
#include "ServerModule/GameSession/RPCRouterComponent.h"

#include <ServerModule/GameSession/GameSessionState.h>
#include <ServerModule/GameSession/PlayerSessionState.h>

#include "ServerModule/Sound/AmbientAudioManager.h"
#include "Kismet/GameplayStatics.h"

AGOCLEANPlayerController::AGOCLEANPlayerController()
{
	RPCRouter = CreateDefaultSubobject<URPCRouterComponent>(TEXT("RPCRouter"));
}


void AGOCLEANPlayerController::BeginPlay()
{
	Super::BeginPlay();


    if (IsLocalController() && GetWorld()->GetMapName() == TEXT("FirstPersonMap"))
    {
        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->RemoveAllViewportWidgets();
            UE_LOG(LogTemp, Log, TEXT("[Server/Client] Force Clear All Widgets in New Level"));
        }
    }


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
void AGOCLEANPlayerController::CloseLobbyUI()
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
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

void AGOCLEANPlayerController::Client_ShowLobbyUI_Implementation()
{
    ShowLobbyUI();
}

void AGOCLEANPlayerController::Client_CloseLobbyUI_Implementation()
{
    CloseLobbyUI();
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





void AGOCLEANPlayerController::EnsureVendingWidget()
{
    if (!IsLocalController()) return; 
    if (VendingWidget) return;
    if (!VendingWidgetClass) return;

    VendingWidget = CreateWidget<UUserWidget>(this, VendingWidgetClass);
    if (VendingWidget)
    {
        VendingWidget->AddToViewport();
        VendingWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool AGOCLEANPlayerController::IsVendingUIOpen() const
{
    return VendingWidget && VendingWidget->GetVisibility() == ESlateVisibility::Visible;
}

void AGOCLEANPlayerController::OpenVendingUI()
{
    if (!IsLocalController()) return;

    EnsureVendingWidget();
    if (!VendingWidget) return;

    VendingWidget->SetVisibility(ESlateVisibility::Visible);

    bShowMouseCursor = true;
    FInputModeUIOnly Mode;
    SetInputMode(Mode);
}

void AGOCLEANPlayerController::CloseVendingUI()
{
    if (!IsLocalController()) return;
    if (!VendingWidget) return;

    VendingWidget->SetVisibility(ESlateVisibility::Hidden);

    bShowMouseCursor = false;
    FInputModeGameOnly Mode;
    SetInputMode(Mode);
}

void AGOCLEANPlayerController::ToggleVendingUI()
{
    if (IsVendingUIOpen())
    {
        CloseVendingUI();
    }
    else
    {
        OpenVendingUI();
    }
}



void AGOCLEANPlayerController::RequestTogglePurchasedVending(int32 ItemId)
{
    Server_TogglePurchasedVending(ItemId);
}

int32 AGOCLEANPlayerController::GetMySeatIndex_ServerSafe() const
{
    const APlayerSessionState* PSS = Cast<APlayerSessionState>(PlayerState);
    return PSS ? PSS->GetSeatIndex() : INDEX_NONE;
}

void AGOCLEANPlayerController::Server_TogglePurchasedVending_Implementation(int32 ItemId)
{
    AGameSessionState* GS = GetWorld() ? GetWorld()->GetGameState<AGameSessionState>() : nullptr;
    if (!GS)
        return;

    const int32 PlayerId = GetMySeatIndex_ServerSafe();
    if (PlayerId == INDEX_NONE)
        return;

    GS->TogglePurchasedVending(PlayerId, ItemId);
}
