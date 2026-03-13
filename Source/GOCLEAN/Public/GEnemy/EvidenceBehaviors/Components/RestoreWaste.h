/**
 * @class URestoreWaste
 * @brief 단서 행동: 폐기물 복구
 *
 * **[Core functions]**
 * - ExecuteBehavior(AActor* GhostActor): 행동 실행
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/EvidenceBehaviors/Base/UEvidenceBehavior.h"
#include "RestoreWaste.generated.h"

UCLASS()
class GOCLEAN_API URestoreWaste : public UEvidenceBehavior
{
	GENERATED_BODY()
	
public:
	virtual void ExecuteBehavior(AActor* GhostActor) override;
};
