// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FUIData.h"
#include "Blueprint/UserWidget.h"
#include "GUIBase.h"
#include "GUILayer.h"
#include "GUIPopup.h"
#include "GUIManager.generated.h"

class UGUIAction;
class UGUILayer;
class UGUIPopup;

UCLASS()
class GOCLEAN_API UGUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	


// Overrided funcitons //
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;



// Funtions //
public:
	UUserWidget* ShowUI(FName UIID);
	UUserWidget* CreateUI(FName UIID);
	void HideUI(FName UIID);
	UUserWidget* GetUIWidget(FName UIID);
	void BroadcastToUI(FName UIID, UObject* Data);
	void PlayAction(FName UIID, UGUIAction* Action);
	
	

// Variables //
private:
	UPROPERTY()
	TObjectPtr<UDataTable> UIRegistrationTable;

	UPROPERTY()
	TMap<FName, TObjectPtr<UGUILayer>> LayerMap;
	UPROPERTY()
	TMap<FName, TObjectPtr<UGUIPopup>> PopupMap;

	UPROPERTY()
	TArray<TObjectPtr<UGUIPopup>> PopupStack;
};
