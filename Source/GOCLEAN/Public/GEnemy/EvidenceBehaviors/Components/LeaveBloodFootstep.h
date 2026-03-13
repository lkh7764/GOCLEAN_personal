/**
 * @class ULeaveBloodFootstep
 * @brief 단서 행동: 핏발자국
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "LeaveBloodFootstep.generated.h"

UCLASS()
class GOCLEAN_API ULeaveBloodFootstep : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};