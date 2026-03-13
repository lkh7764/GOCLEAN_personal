/**
* @class UGUIBase
* @brief UIID 기반으로 관리되는, UI들의 최상위 class
* 
* **[Core functions]**
* GetUIID() const: UIID 반환
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GUIBase.generated.h"

UCLASS(Abstract)
class GOCLEAN_API UGUIBase : public UUserWidget
{
	GENERATED_BODY()



// Funcitons //
protected:
	UPROPERTY(EditDefaultsOnly)
	FName UIID;

public:
	virtual void OnSetup();

// Getter, Setter
public:
	FName GetUIID() const { return UIID; };
};