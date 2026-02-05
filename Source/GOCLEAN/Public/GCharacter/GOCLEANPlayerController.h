// Copyright Epic Games, Inc. All Rights Reserved.
/**
 * @class AGOCLEANPlayerController
 * @brief Player controller
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GOCLEANPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class GOCLEAN_API AGOCLEANPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface
};
