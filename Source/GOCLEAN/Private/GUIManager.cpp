// Fill out your copyright notice in the Description page of Project Settings.


#include "GUIManager.h"

// Overrided funcitons //
void UGUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UIRegistrationTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Seohyeon/UI/DT_UIRegistration.DT_UIRegistration'"));
	
	if (!UIRegistrationTable) UE_LOG(LogTemp, Error, TEXT("Failed to load UIRegistrationTable | GUIManager.cpp"));
	UE_LOG(LogTemp, Log, TEXT("UIRegistrationTable loaded | GUIManager.cpp"));
}
void UGUIManager::Deinitialize()
{
	Super::Deinitialize();
}



// Funtions //
// JSH MEMO: 이것도 굳이 UUserWidget*을 반환해야 할까?
UUserWidget* UGUIManager::ShowUI(FName UIID)
{
	FUIData* UIDataRow = UIRegistrationTable->FindRow<FUIData>(UIID, TEXT("[UI manager]"));
	//if (!UIDataRow) UE_LOG(LogTemp, Warning, TEXT("No UIDataRow for %s | GUIManager.cpp"), UIID);

	if (UIDataRow->UIType == EUIType::Layer)
	{
		if (TObjectPtr<UGUILayer>* LayerUI = LayerMap.Find(UIID)) {
			(*LayerUI)->SetVisibility(ESlateVisibility::Visible);
			(*LayerUI)->AddToViewport(UIDataRow->ZOrder);

			return *LayerUI;
		}
		else { 
			UUserWidget* WidgetInstance = CreateUI(UIID);
			LayerMap.Add(UIID, Cast<UGUILayer>(WidgetInstance));
			WidgetInstance->AddToViewport(UIDataRow->ZOrder);

			return WidgetInstance;
		}
	}
	else if (UIDataRow->UIType == EUIType::Popup)
	{
		if (TObjectPtr<UGUIPopup>* PopupUI = PopupMap.Find(UIID)) {
			(*PopupUI)->SetVisibility(ESlateVisibility::Visible);
			(*PopupUI)->AddToViewport(UIDataRow->ZOrder);

			return *PopupUI;
		}
		else {
			UUserWidget* WidgetInstance = CreateUI(UIID);
			PopupMap.Add(UIID, Cast<UGUIPopup>(WidgetInstance));
			PopupStack.Push(Cast<UGUIPopup>(WidgetInstance));
			WidgetInstance->AddToViewport(UIDataRow->ZOrder);

			return WidgetInstance;
		}
	}
	
	return nullptr;
}
UUserWidget* UGUIManager::CreateUI(FName UIID)
{
	FUIData* UIDataRow = UIRegistrationTable->FindRow<FUIData>(UIID, TEXT("[UI manager]"));
	//if (!UIDataRow) UE_LOG(LogTemp, Warning, TEXT("No UIDataRow for %s | GUIManager.cpp"), UIID);
	
	UClass* LoadedWidgetBluePrintClass = UIDataRow->WidgetBluePrintClass.LoadSynchronous();

	UGUIBase* WidgetInstance = CreateWidget<UGUIBase>(GetWorld(), LoadedWidgetBluePrintClass);
	
	// JSH MEMO: OnSetup() 말고, 생성자에서 처리해도 되지 않나.
	// 아니면 Setup 말고, Initialization이 나을 수도. 반복해서 쓰게 된다면.
	WidgetInstance->OnSetup();

	return WidgetInstance;
}
void UGUIManager::HideUI(FName UIID)
{
	FUIData* UIDataRow = UIRegistrationTable->FindRow<FUIData>(UIID, TEXT("[UI manager]"));
	//if (!UIDataRow) UE_LOG(LogTemp, Warning, TEXT("No UIDataRow for %s | GUIManager.cpp"), UIID);

	if (UIDataRow->UIType == EUIType::Layer)
	{
		TObjectPtr<UGUILayer>* LayerUI = LayerMap.Find(UIID);
		if (LayerUI == nullptr) return;

		(*LayerUI)->SetVisibility(ESlateVisibility::Hidden);
	}

	if (UIDataRow->UIType == EUIType::Popup)
	{
		TObjectPtr<UGUIPopup>* PopupUI = PopupMap.Find(UIID);
		if (PopupUI == nullptr) return;

		(*PopupUI)->SetVisibility(ESlateVisibility::Hidden);
	}
}
UUserWidget* UGUIManager::GetUIWidget(FName UIID)
{
	FUIData* UIDataRow = UIRegistrationTable->FindRow<FUIData>(UIID, TEXT("[UI manager]"));
	// 경우에 수에 따른 로그 출력, 예외 처리, 주석. 어떤 균형으로 코드를 작성해야 진정으로 좋은 코드려나.
	//if (!UIDataRow) UE_LOG(LogTemp, Warning, TEXT("No UIDataRow for %s | GUIManager.cpp"), UIID);

	if (UIDataRow->UIType == EUIType::Layer)
	{
		TObjectPtr<UGUILayer>* LayerUI = LayerMap.Find(UIID);
		if (LayerUI == nullptr) return nullptr;
		
		return *LayerUI;
	}
	else if (UIDataRow->UIType == EUIType::Popup)
	{
		TObjectPtr<UGUIPopup>* PopupUI = PopupMap.Find(UIID);
		if (PopupUI == nullptr) return nullptr;

		return *PopupUI;
	}

	return nullptr;
}


void UGUIManager::BroadcastToUI(FName UIID, UObject* Data)
{
	;
}
void UGUIManager::PlayAction(FName UIID, UGUIAction* Action)
{
	;
}