/**
 * @class URemoveShadow
 * @brief 단서 행동: 그림자 소멸
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "RemoveShadow.generated.h"

UCLASS()
class GOCLEAN_API URemoveShadow : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
