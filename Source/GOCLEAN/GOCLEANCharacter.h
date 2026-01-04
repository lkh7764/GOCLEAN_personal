// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputAction.h"

#include "CharacterStatsComponent.h"

#include "GOCLEANCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpotLightComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class GOCLEAN_API AGOCLEANCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGOCLEANCharacter();


	// Getter, Setter //
	float GetPlayerCurrentSanity() const;
	void SetPlayerCurrentSanity(float NewPlayerCurrentSanity);


private:
	// Components //
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpotLightComponent> FlashlightComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCharacterStatsComponent> StatsComp;

	// Input Actions //
	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> FlashlightAction;




	// Overrided //
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	void Jump() override;


	// Actions //
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Crouch();

	void Sprint();
	void SprintRelease();

	void StartStaminaRecovery();
	void RecoverStamina();

	void ToggleFlashlight();




	// Handles //
	FTimerHandle StaminaRecoveryHandle;


	// States //
	bool bIsRecoveringStamina;
	bool bIsCrouching;
	bool bIsSprinting;

};