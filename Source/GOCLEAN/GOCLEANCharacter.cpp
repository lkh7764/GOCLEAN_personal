// Copyright Epic Games, Inc. All Rights Reserved.

#include "GOCLEANCharacter.h"
#include "GOCLEANProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGOCLEANCharacter

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
}

void AGOCLEANCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

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
	//UE_LOG(LogTemp, Warning, TEXT("Crouch() 호출. bIsCrouching 값: %s"), bIsCrouching ? TEXT("true") : TEXT("false"));
	if (GetCharacterMovement()->IsFalling()) return;

	float StandingCapsuleHalfHeight = 96.0f; // 캡슐 높이 : Stand 상태
	float CrouchingCapsuleHalfHeight = 48.0f; // 캡슐 높이 : Crouch 상태
	
	if (bIsCrouching) 
	{
		// Standing
		GetCapsuleComponent()->SetCapsuleHalfHeight(StandingCapsuleHalfHeight); // 컴포넌트 캡슐 높이 조정
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f)); // 카메라 위치 조정
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

		bIsCrouching = false;
	}
	else
	{
		// Crouching
		GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchingCapsuleHalfHeight); // 컴포넌트 캡슐 높이 조정
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.0f, 0.0f, 30.0f)); // 카메라 위치 조정
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;

		bIsCrouching = true;
	}
}

// Sprint
void AGOCLEANCharacter::Sprint()
{
	//if(bIsCrouching) UE_LOG(LogTemp, Warning, TEXT("Crouch() 호출. bIsCrouching 값: %s"), bIsCrouching ? TEXT("true") : TEXT("false"));

	if (bIsCrouching || GetCharacterMovement()->IsFalling()) return;

	if (CurrentStamina <= 0.0f) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	// Stamina 회복 중지
	bIsRecoveringStamina = false;
	GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
}

void AGOCLEANCharacter::SprintRelease()
{
	if (bIsCrouching) return;

	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Stamina 회복 시작
	if (bIsRecoveringStamina == false) {
		bIsRecoveringStamina = true;
		UE_LOG(LogTemp, Log, TEXT("Stamina 회복 시작"));
		GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AGOCLEANCharacter::StartStaminaRecovery, RecoveryDelay, false);
	}
}

// ACharacter Jump() 오버라이딩
void AGOCLEANCharacter::Jump()
{
	if (bIsCrouching) return;

	Super::Jump();
}

void AGOCLEANCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSprinting) {
		CurrentStamina -= StaminaDrainRate * DeltaTime;
		UE_LOG(LogTemp, Log, TEXT("bIsSprinting : CurrentStamina 값 : %f"), CurrentStamina);

		if (CurrentStamina <= 0.0f) {
			CurrentStamina = 0.0f;
			SprintRelease();
		}
	}
}

void AGOCLEANCharacter::StartStaminaRecovery()
{
	// UE_LOG(LogTemp, Log, TEXT("Stamina 회복 시작")); 이거 왜 체크가 안되지
	GetWorldTimerManager().SetTimer(StaminaRecoveryHandle, this, &AGOCLEANCharacter::RecoverStamina, 0.1f, true);
}

void AGOCLEANCharacter::RecoverStamina() 
{
	CurrentStamina += StaminaRecoveryRate * 0.1f;
	UE_LOG(LogTemp, Log, TEXT("bIsRecoveringStamina : CurrentStamina 값 : %f"), CurrentStamina);
	if (CurrentStamina >= MaxStamina) {
		CurrentStamina = MaxStamina;
		bIsRecoveringStamina = false;
		UE_LOG(LogTemp, Log, TEXT("Stamina 회복 종료"));
		GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
	}
}