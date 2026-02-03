/**
* @struct FUIData
* @brief UIID 기반으로 UI를 관리하기 위한 DataTable에서, 각 UI가 필요로 하는 Data의 형식들을 정리해둔 구조체
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FUIData.generated.h"

class UGUIBase;

UENUM(BlueprintType)
enum class EUIType : uint8 { Layer, Popup };

USTRUCT(BlueprintType)
struct GOCLEAN_API FUIData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EUIType UIType = EUIType::Layer;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGUIBase> WidgetBluePrintClass = nullptr;

	UPROPERTY(EditAnywhere)
	int32 ZOrder = 0;
};