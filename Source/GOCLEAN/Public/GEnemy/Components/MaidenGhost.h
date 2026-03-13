/**
 * @class AMaidenGhost
 * @brief ¼Õ°¢½Ã
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/Base/GhostBase.h"
#include "MaidenGhost.generated.h"

UCLASS()
class GOCLEAN_API AMaidenGhost : public AGhostBase
{
	GENERATED_BODY()

public:
	AMaidenGhost();
	
	virtual void BeginPlay() override;
};
