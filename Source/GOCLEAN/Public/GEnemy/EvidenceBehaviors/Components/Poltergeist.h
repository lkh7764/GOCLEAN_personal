/**
 * @class UPoltergeist
 * @brief 단서 행동: 물건 던지기
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "Poltergeist.generated.h"

UCLASS()
class GOCLEAN_API UPoltergeist : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
