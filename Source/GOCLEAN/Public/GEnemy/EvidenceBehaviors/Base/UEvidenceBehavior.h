/**
 * @class UEvidenceBehavior
 * @brief 단서 행동 Base class
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor PURE_VIRTUAL(UEvidenceBehavior::Execute, )): 순수 가상 함수
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UEvidenceBehavior.generated.h"

UCLASS(Abstract)
class GOCLEAN_API UEvidenceBehavior : public UObject
{
	GENERATED_BODY()


public:
	virtual void ExecuteBehavior(AActor* GhostActor) PURE_VIRTUAL(UEvidenceBehavior::Execute, );
};