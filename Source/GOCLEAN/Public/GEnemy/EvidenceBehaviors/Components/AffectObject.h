/**
 * @class UAffectObject
 * @brief 단서 행동: 오브젝트 영향
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "AffectObject.generated.h"

UCLASS()
class GOCLEAN_API UAffectObject : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
