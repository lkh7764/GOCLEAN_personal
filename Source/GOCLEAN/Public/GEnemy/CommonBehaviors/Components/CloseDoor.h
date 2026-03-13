/**
 * @class UCloseDoor
 * @brief 공통 행동
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/CommonBehaviors/Base/CommonBehavior.h"
#include "CloseDoor.generated.h"

UCLASS()
class GOCLEAN_API UCloseDoor : public UCommonBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
