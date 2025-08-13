// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
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

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	// Flashlight
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flashlight, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* FlashlightComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Crouch Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	// Sprint Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	// Flashlight Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FlashlightAction;
	
public:
	AGOCLEANCharacter();

protected:
	virtual void BeginPlay();

	//능력치 선언
	
	// Life
	int Life;

	// Sanity
	float MaxSanity;
	float CurrentSanity;

	// Stamina
	float MaxStamina;
	float CurrentStamina;
	float StaminaDrainRate;
	float StaminaRecoveryRate;
	float RecoveryDelay;
	bool bIsRecoveringStamina;

	FTimerHandle StaminaRecoveryHandle;

	// Speed
	float WalkSpeed;
	float CrouchSpeed;
	float SprintSpeed;
	float SpeedModifier;

	// State
	bool bIsCrouching;
	bool bIsSprinting;

public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Crouch
	virtual void Crouch();

	// Sprint
	void Sprint();
	void SprintRelease();

	// Stamina
	void StartStaminaRecovery();
	void RecoverStamina();

	// Flashlight
	void ToggleFlashlight();

	// ACharacter 함수 오버라이드
	virtual void Jump() override;
	virtual void Tick(float DeltaTime) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	float GetCurrentSanity() const;
	void DecreaseLife(int Amount);
	void IncreaseSanity(float Amount);
	void DecreaseSanity(float Amount);
	void IncreaseSpeedModifier(float Amount);
	void DecreaseSpeedModifier(float Amount);
};