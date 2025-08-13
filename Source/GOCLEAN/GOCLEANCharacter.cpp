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
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AGOCLEANCharacter::AGOCLEANCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	//Flashlight
	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	FlashlightComponent->SetupAttachment(FirstPersonCameraComponent);
	FlashlightComponent->Intensity = 50.f;
	FlashlightComponent->IntensityUnits = ELightUnits::Lumens;
	FlashlightComponent->AttenuationRadius = 800.f;
	FlashlightComponent->InnerConeAngle = 1.f;
	FlashlightComponent->OuterConeAngle = 30.f;
	FlashlightComponent->SoftSourceRadius = 500.f;
	FlashlightComponent->SetLightColor(FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("FFF2D6FF"))));

	// Life
	Life = 2;

	// Sanity
	MaxSanity = 100.0f;
	CurrentSanity = MaxSanity;

	// Stamina
	MaxStamina = 10.0f;
	CurrentStamina = MaxStamina;
	StaminaDrainRate = 1.0f;
	StaminaRecoveryRate = 1.0f;
	RecoveryDelay = 1.0f;
	bIsRecoveringStamina = false;

	// Speed
	WalkSpeed = 600.0f;
	CrouchSpeed = 300.0f;
	SprintSpeed = 900.0f;
	SpeedModifier = 0.0f;

	// State
	bIsCrouching = false;
	bIsSprinting = false;
}

void AGOCLEANCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();
}

// Enhanced Input: Binding Action

void AGOCLEANCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGOCLEANCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGOCLEANCharacter::Look);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::Crouch);

		// Sprint & SprintRelease
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGOCLEANCharacter::SprintRelease);

		// Flashlight
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::ToggleFlashlight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGOCLEANCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGOCLEANCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGOCLEANCharacter::Crouch()
{
	if (GetCharacterMovement()->IsFalling()) return;

	float StandingCapsuleHalfHeight = 96.0f; // Default State
	float CrouchingCapsuleHalfHeight = 48.0f; // Crouching State
	
	if (bIsCrouching) 
	{
		// Standing
		GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHalfHeight);
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed + SpeedModifier;

		bIsCrouching = false;
	}
	else
	{
		// Crouching
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchingCapsuleHalfHeight);
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 0.0f, 30.0f));
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed + SpeedModifier;

		bIsCrouching = true;
	}
}

// Sprint
void AGOCLEANCharacter::Sprint()
{
	if (bIsCrouching || GetCharacterMovement()->IsFalling()) return;

	if (CurrentStamina <= 0.0f) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed + SpeedModifier;

	bIsRecoveringStamina = false;
	GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
}

void AGOCLEANCharacter::SprintRelease()
{
	if (bIsCrouching) return;

	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed + SpeedModifier;

	if (bIsRecoveringStamina == false) {
		bIsRecoveringStamina = true;
		//UE_LOG(LogTemp, Log, TEXT("Stamina Recover Start"));
		GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AGOCLEANCharacter::StartStaminaRecovery, RecoveryDelay, false);
	}
}

void AGOCLEANCharacter::StartStaminaRecovery()
{
	// UE_LOG(LogTemp, Log, TEXT("Stamina Recover Start"));
	GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AGOCLEANCharacter::RecoverStamina, 0.1f, true);
}

void AGOCLEANCharacter::RecoverStamina() 
{
	CurrentStamina += StaminaRecoveryRate * 0.1f;
	//UE_LOG(LogTemp, Log, TEXT("bIsRecoveringStamina : CurrentStamina : %f"), CurrentStamina);
	if (CurrentStamina >= MaxStamina) {
		CurrentStamina = MaxStamina;
		bIsRecoveringStamina = false;
		//UE_LOG(LogTemp, Log, TEXT("Stamina Recover End"));
		GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
	}
}

void AGOCLEANCharacter::ToggleFlashlight()
{
	FlashlightComponent->ToggleVisibility();
}

float AGOCLEANCharacter::GetCurrentSanity() const { return CurrentSanity; }
void AGOCLEANCharacter::DecreaseLife(int Amount) { Life -= Amount; }
void AGOCLEANCharacter::IncreaseSanity(float Amount) { CurrentSanity += Amount; }
void AGOCLEANCharacter::DecreaseSanity(float Amount) { CurrentSanity -= Amount; }
void AGOCLEANCharacter::IncreaseSpeedModifier(float Amount) { SpeedModifier += Amount; }
void AGOCLEANCharacter::DecreaseSpeedModifier(float Amount) { SpeedModifier -= Amount; }

// Override function

// Jump
void AGOCLEANCharacter::Jump()
{
	if (bIsCrouching) return;

	Super::Jump();
}

// Tick
void AGOCLEANCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting) {
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		UE_LOG(LogTemp, Log, TEXT("bIsSprinting : CurrentStamina : %f"), CurrentStamina);

		if (CurrentStamina <= 0.0f) {
			CurrentStamina = 0.0f;
			SprintRelease();
		}
	}

	// JSH Flag: Sanity
	//UE_LOG(LogTemp, Warning, TEXT("Current Player Sanity: %f"), CurrentSanity);
	DecreaseSanity(0.0167f);
}