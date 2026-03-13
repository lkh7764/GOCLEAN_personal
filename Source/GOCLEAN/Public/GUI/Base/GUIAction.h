/**
* @class UGUIAction
* @brief UI들이 실행하는 Action들의 Abstract class
* 
* **[Core functions]**
* - ExecuteAction(UUserWidget* Target) PURE_VIRTUAL(UUIAction::ExecuteAction, ): Action 실행
* - IsFinished() const: Action이 종료되었는지 판단값 반환
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GUIAction.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew) // DefaultToInstanced
class GOCLEAN_API UGUIAction : public UObject
{
	GENERATED_BODY()
	


// Funcitons //
// JSH MEMO: Change access specifier public to private & declare friend class with GUIManager?
public:
	virtual void ExecuteAction(UUserWidget* Target) PURE_VIRTUAL(UUIAction::ExecuteAction, );

	virtual bool IsFinished() const { return true; };
};