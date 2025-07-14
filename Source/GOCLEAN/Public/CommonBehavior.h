#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonBehavior.generated.h"

UCLASS(Abstract)
class GOCLEAN_API UCommonBehavior : public UObject
{
	GENERATED_BODY()


public:
	virtual void ExecuteBehavior(AActor* GhostActor) PURE_VIRTUAL(UCommonBehavior::Execute, );
};
