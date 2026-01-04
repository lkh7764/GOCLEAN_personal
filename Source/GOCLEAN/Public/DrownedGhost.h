#pragma once

#include "CoreMinimal.h"
#include "GhostBase.h"
#include "DrownedGhost.generated.h"

/**
 *
 */
UCLASS()
class GOCLEAN_API ADrownedGhost : public AGhostBase
{
	GENERATED_BODY()


public:
	ADrownedGhost();

	virtual void BeginPlay() override;
};