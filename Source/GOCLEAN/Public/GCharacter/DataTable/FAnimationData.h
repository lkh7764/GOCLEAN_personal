/**
* @struct FUIData
* @brief AnimID 기반으로 Animation 관리하기 위한 DataTable에서, Data의 형식들을 정리해둔 구조체
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FAnimationData.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct GOCLEAN_API FAnimationData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 AnimID;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;
};