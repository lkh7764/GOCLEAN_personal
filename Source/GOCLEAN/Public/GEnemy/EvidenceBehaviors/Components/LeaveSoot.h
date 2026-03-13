/**
 * @class ULeaveSoot
 * @brief 단서 행동: 그을음
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "LeaveSoot.generated.h"

UCLASS()
class GOCLEAN_API ULeaveSoot : public UEvidenceBehavior
{
	GENERATED_BODY()

public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
