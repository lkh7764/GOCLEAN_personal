// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GOCLEANCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGOCLEANCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Crouch Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	// Sprint Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	
public:
	AGOCLEANCharacter();

protected:
	virtual void BeginPlay();

	//캐릭터 기본 스탯 선언 & 정의
	
	// Sanity
	float MaxSanity = 100.0f;
	float CurrentSanity = MaxSanity;

	// Stamina
	float MaxStamina = 10.0f;
	float CurrentStamina = MaxStamina;
	float StaminaDrainRate = 1.0f;
	float StaminaRecoveryRate = 1.0f;
	float RecoveryDelay = 1.0f;
	bool bIsRecoveringStamina = false;

	FTimerHandle StaminaRecoveryHandle;

	// Speed
	float WalkSpeed = 600.0f;
	float CrouchSpeed = 300.0f;
	float SprintSpeed = 900.0f;

	// State
	bool bIsCrouching = false;
	bool bIsSprinting = false;

	
private:

public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called for crouching input
	virtual void Crouch();

	// Called for sprinting input 
	void Sprint();

	// Sprint() 기능 종료 시 호출
	void SprintRelease();

	// Stamina 관련
	void StartStaminaRecovery();
	void RecoverStamina();

	// ACharacter 함수 오버라이드
	virtual void Jump() override;
	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

