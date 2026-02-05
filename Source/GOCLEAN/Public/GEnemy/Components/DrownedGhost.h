/**
 * @class ADrownedGhost
 * @brief ¼ö»ì±Í
 */

#pragma once

#include "CoreMinimal.h"
#include "GEnemy/Base/GhostBase.h"
#include "DrownedGhost.generated.h"

UCLASS()
class GOCLEAN_API ADrownedGhost : public AGhostBase
{
	GENERATED_BODY()


public:
	ADrownedGhost();

	virtual void BeginPlay() override;
};