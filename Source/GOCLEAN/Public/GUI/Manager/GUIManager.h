/**
* @class UGUIManager
* @brief 프로젝트의 모든 UI 생명주기 및 Layer system을 관리 -> Unreal Subsystem
* 
* **[Core functions]**
* - ShowUI(FName UIID): UIID에 해당하는 UI를 화면에 출력
* - HideUI(FName UIID): UIID에 해당하는 UI를 비가시 처리
* - CreateUI(FName UIID): UIID에 해당하는 UI 객체 생성
* - GetUIWidget(FName UIID): UIID에 해당하는 UI 객체의 포인터를 반환
* - BroadcastToUI(FName UIID, UObject* Data): UIID에 해당하는 UI 객체에게 데이터 전달
* - PlayAction(FName UIID, UGUIAction* Action): UIID의 Action 실행
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GUI/DataTable/FUIData.h"
#include "Blueprint/UserWidget.h"
#include "GUI/Base/GUIBase.h"
#include "GUI/Base/GUILayer.h"
#include "GUI/Base/GUIPopup.h"
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
