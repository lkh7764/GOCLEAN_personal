/**
 * @class UTurnOffLight
 * @brief 단서 행동: 불 끄기
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "TurnOffLight.generated.h"

UCLASS()
class GOCLEAN_API UTurnOffLight : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
