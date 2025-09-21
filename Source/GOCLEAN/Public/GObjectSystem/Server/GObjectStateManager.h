#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GOCLEAN/Public/GObjectSystem/Server/GFixedObjectState.h"
#include "GOCLEAN/Public/GObjectSystem/Server/GNonfixedObjectState.h"
#include "GObjectStateManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UGObjectStateManager : public UActorComponent
{
	GENERATED_BODY()



	// constructor
public:	
	UGObjectStateManager();



	// variable
protected:



	// functions
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
