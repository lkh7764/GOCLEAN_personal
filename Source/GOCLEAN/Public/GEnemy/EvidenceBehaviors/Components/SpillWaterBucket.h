/**
 * @class USpillWaterBucket
 * @brief 단서 행동: 물통 쏟기
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "SpillWaterBucket.generated.h"

UCLASS()
class GOCLEAN_API USpillWaterBucket : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
