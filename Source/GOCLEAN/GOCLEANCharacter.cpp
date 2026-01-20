// Copyright Epic Games, Inc. All Rights Reserved.

#include "GOCLEANCharacter.h"
#include "GOCLEANProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GUIManager.h"
#include "GameFramework/CharacterMovementComponent.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AGOCLEANCharacter::AGOCLEANCharacter()
{	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMeshComp"));
	MeshComp->SetOnlyOwnerSee(true);
	MeshComp->SetupAttachment(CameraComp);
	MeshComp->bCastDynamicShadow = false;
	MeshComp->CastShadow = false;
	MeshComp->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Components //
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	CameraComp->bUsePawnControlRotation = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FlashlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComp"));
	FlashlightComp->SetupAttachment(CameraComp);
	FlashlightComp->Intensity = 50.f;
	FlashlightComp->IntensityUnits = ELightUnits::Lumens;
	FlashlightComp->AttenuationRadius = 800.f;
	FlashlightComp->InnerConeAngle = 1.f;
	FlashlightComp->OuterConeAngle = 30.f;
	FlashlightComp->SoftSourceRadius = 500.f;
	FlashlightComp->SetLightColor(FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("FFF2D6FF"))));

	StatsComp = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StasComp"));


	// States //
	bIsRecoveringStamina = false;
	bIsCrouching = false;
	bIsSprinting = false;
}


// Getter, Setter //
float AGOCLEANCharacter::GetPlayerCurrentSanity() const { return StatsComp ? StatsComp->GetCurrentSanity() : 0.0f; }
void AGOCLEANCharacter::SetPlayerCurrentSanity(float NewPlayerCurrentSanity) { 
	if (StatsComp == nullptr) return;
	StatsComp->SetCurrentSanity(NewPlayerCurrentSanity);
}


// Overrided //
void AGOCLEANCharacter::Tick(float DeltaTime)
{
	if (StatsComp == nullptr) return;

	Super::Tick(DeltaTime);

	if (bIsSprinting) {
		//JSH TEMP: CurrentStamina -= StaminaDrainRate * DeltaTime;
		StatsComp->DecreaseCurrentStamina(StatsComp->GetStaminaDrainRate() * DeltaTime);
		UE_LOG(LogTemp, Log, TEXT("bIsSprinting : CurrentStamina : %f"), StatsComp->GetCurrentStamina());

		if (StatsComp->GetCurrentStamina() <= 0.0f) {
			StatsComp->SetCurrentStamina(0.0f);
			SprintRelease();
		}
	}

	// JSH Flag: Sanity
	//UE_LOG(LogTemp, Warning, TEXT("Current Player Sanity: %f"), CurrentSanity);
	StatsComp->DecreaseCurrentSanity(0.0167f);
	//StatsComp->SetCurrentSanity(StatsComp->GetCurrentSanity());
}

void AGOCLEANCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGOCLEANCharacter::Move);

		// Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGOCLEANCharacter::Look);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::Crouch);

		// Sprint & SprintRelease
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGOCLEANCharacter::SprintRelease);

		// Flashlight toggle
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::ToggleFlashlight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component."), *GetNameSafe(this));
	}
}

void AGOCLEANCharacter::Jump()
{
	if (bIsCrouching) return;

	Super::Jump();

	// Tmp
	GetGameInstance()->GetSubsystem<UGUIManager>()->ShowUI(TEXT("HUD"));
}


// Actions //
void AGOCLEANCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGOCLEANCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGOCLEANCharacter::Crouch()
{
	if (StatsComp == nullptr) return;

	if (GetCharacterMovement()->IsFalling()) return;

	float StandingCapsuleHalfHeight = 96.0f;
	float CrouchingCapsuleHalfHeight = 48.0f;

	if (bIsCrouching)
	{
		// Standing
		GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHalfHeight);
		CameraComp->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetWalkSpeed();

		bIsCrouching = false;
	}
	else
	{
		// Crouching
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchingCapsuleHalfHeight);
		CameraComp->SetRelativeLocation(FVector(-10.0f, 0.0f, 30.0f));
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetCrouchSpeed();

		bIsCrouching = true;
	}
}

void AGOCLEANCharacter::Sprint()
{
	if (StatsComp == nullptr) return;

	if (bIsCrouching || GetCharacterMovement()->IsFalling()) return;

	if (StatsComp->GetCurrentStamina() <= 0.0f) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetSprintSpeed();

	bIsRecoveringStamina = false;
	GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
}

void AGOCLEANCharacter::SprintRelease()
{
	if (StatsComp == nullptr) return;

	if (bIsCrouching) return;

	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetWalkSpeed();

	if (bIsRecoveringStamina == false) {
		bIsRecoveringStamina = true;
		//UE_LOG(LogTemp, Log, TEXT("Stamina Recover Start"));
		GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AGOCLEANCharacter::StartStaminaRecovery, StatsComp->GetStaminaRecoveryDelay(), false);
	}
}

void AGOCLEANCharacter::StartStaminaRecovery()
{
	// UE_LOG(LogTemp, Log, TEXT("Stamina Recover Start"));
	GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AGOCLEANCharacter::RecoverStamina, 0.1f, true);
}

void AGOCLEANCharacter::RecoverStamina() 
{
	if (StatsComp == nullptr) return;

	// JSH TEMP: CurrentStamina += StaminaRecoveryRate * 0.1f;
	StatsComp->IncreaseCurrentStamina(StatsComp->GetStaminaRecoveryRate() * 0.1f);

	//UE_LOG(LogTemp, Log, TEXT("bIsRecoveringStamina : CurrentStamina : %f"), CurrentStamina);
	if (StatsComp->GetCurrentStamina() >= StatsComp->GetMaxStamina()) {
		StatsComp->SetCurrentStamina(StatsComp->GetMaxStamina());
		bIsRecoveringStamina = false;
		//UE_LOG(LogTemp, Log, TEXT("Stamina Recover End"));
		GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
	}
}

void AGOCLEANCharacter::ToggleFlashlight()
{
	FlashlightComp->ToggleVisibility();
}