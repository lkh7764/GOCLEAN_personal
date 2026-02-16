// Copyright Epic Games, Inc. All Rights Reserved.

#include "GCharacter/GOCLEANCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GUI/Manager/GUIManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Animation/AnimSingleNodeInstance.h"

#include "GPlayerSystem/GEquipment/GEquipmentComponent.h"
#include "GPlayerSystem/InteractionComponent.h"
#include "GTypes/IGInteractable.h"
#include "Net/UnrealNetwork.h"
#include "GDataManagerSubsystem.h"
#include "GTypes/DataTableRow/GEquipmentDataRow.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AGOCLEANCharacter::AGOCLEANCharacter()
{	
	// Components //
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	CameraComp->bUsePawnControlRotation = true;

	FirstPersonMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterFirstPersonMeshComp"));
	FirstPersonMeshComp->SetOnlyOwnerSee(true);
	FirstPersonMeshComp->SetupAttachment(CameraComp);
	FirstPersonMeshComp->bCastDynamicShadow = false;
	FirstPersonMeshComp->CastShadow = false;
	FirstPersonMeshComp->SetRelativeLocation(FVector(10.f, 0.f, -160.f));

	ThirdPersonMeshComp = GetMesh();
	ThirdPersonMeshComp->SetupAttachment(GetCapsuleComponent());
	ThirdPersonMeshComp->SetOwnerNoSee(true);
	ThirdPersonMeshComp->CastShadow = true;
	ThirdPersonMeshComp->bCastDynamicShadow = true;
	ThirdPersonMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	FlashlightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashlightComp"));
	FlashlightComp->SetupAttachment(CameraComp);
	FlashlightComp->Intensity = 35.0f;
	FlashlightComp->IntensityUnits = ELightUnits::Lumens;
	FlashlightComp->AttenuationRadius = 1000.0f;
	FlashlightComp->InnerConeAngle = 15.0f;
	FlashlightComp->OuterConeAngle = 45.0f;
	FlashlightComp->SoftSourceRadius = 500.0f;
	FlashlightComp->SetLightColor(FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("F2E0B8FF"))));

	StatsComp = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("StasComp"));

	// States //
	AnimState = EPlayerAnimState::Stand;
	bIsRecoveringStamina = false;
	bIsSprinting = false;


	InteractionComp = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	EquipComp = CreateDefaultSubobject<UGEquipmentComponent>(TEXT("EquipmentComponent"));


	// Level sequence
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

	StatsComp->DecreaseCurrentSanity(StatsComp->GetSanityDrainRate() * DeltaTime);
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
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::TryCrouch);

		// Sprint & SprintRelease
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::TrySprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGOCLEANCharacter::TrySprintRelease);

		// Flashlight toggle
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::TryToggleFlashlight);


		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AGOCLEANCharacter::DoInteraction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component."), *GetNameSafe(this));
	}
}

// Server //
void AGOCLEANCharacter::TryCrouch()
{
	Server_RequestCrouch();
}
void AGOCLEANCharacter::TrySprint()
{
	Server_RequestSprint();
}
void AGOCLEANCharacter::TrySprintRelease()
{
	Server_RequestSprintRelease();
}
void AGOCLEANCharacter::TryToggleFlashlight()
{
	Server_RequestToggleFlashlight();
}
void AGOCLEANCharacter::TryPlayerInteractionAnim()
{
	Server_RequestPlayerInteractionAnim();
}

void AGOCLEANCharacter::Server_RequestCrouch_Implementation()
{
	Multicast_Crouch();
}
void AGOCLEANCharacter::Server_RequestSprint_Implementation()
{
	Multicast_Sprint();
}
void AGOCLEANCharacter::Server_RequestSprintRelease_Implementation()
{
	Multicast_SprintRelease();
}
void AGOCLEANCharacter::Server_RequestToggleFlashlight_Implementation()
{
	Multicast_ToggleFlashlight();
}
void AGOCLEANCharacter::Server_RequestPlayerInteractionAnim_Implementation()
{
	Multicast_PlayerInteractionAnim();
}
void AGOCLEANCharacter::Server_RequestOnHunted_Implementation()
{
	Multicast_OnHunted();
}
void AGOCLEANCharacter::Server_RequestSetVisible_Implementation(bool IsVisible)
{
	Multicast_SetVisible(IsVisible);
}

void AGOCLEANCharacter::Multicast_ToggleFlashlight_Implementation()
{
	ToggleFlashlight();
}
void AGOCLEANCharacter::Multicast_PlayerInteractionAnim_Implementation()
{
	PlayerInteractionAnim();
}
void AGOCLEANCharacter::Multicast_Crouch_Implementation()
{
	Crouch();
}
void AGOCLEANCharacter::Multicast_Sprint_Implementation()
{
	Sprint();
}
void AGOCLEANCharacter::Multicast_SprintRelease_Implementation()
{
	SprintRelease();
}
void AGOCLEANCharacter::Multicast_OnHunted_Implementation()
{
	OnHunted();
}
void AGOCLEANCharacter::Multicast_SetVisible_Implementation(bool IsVisible)
{
	ThirdPersonMeshComp->SetHiddenInGame(!IsVisible);
}


// OnHunted //

void AGOCLEANCharacter::OnHunted()
{
	UE_LOG(LogTemp, Warning, TEXT("Player Hunted"));
	
	GetCharacterMovement()->StopMovementImmediately();

	FTimerHandle DelayHandle;
	FTimerHandle SpawnDummyDelayHandle;
	FTimerHandle DestroyHandle;
	float AnimationDuration = 3.0f;

	AActor* SpawnedDummyActor = GetWorld()->SpawnActor<AActor>(DummyGhost, GetActorLocation(), GetActorRotation());

	if (ACharacter* DummyGhostCharacter = Cast<ACharacter>(SpawnedDummyActor))
	{
		UAnimationAsset* AnimAsset = DummyGhostCharacter->GetMesh()->GetSingleNodeInstance()->GetCurrentAsset();
		AnimationDuration = AnimAsset->GetPlayLength();
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->SetIgnoreMoveInput(true);
	CameraComp->bUsePawnControlRotation = false;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetWorldTimerManager().SetTimer(DelayHandle, this, &AGOCLEANCharacter::PlayHuntCameraSequence, AnimationDuration - 1.3f, false);
	GetWorldTimerManager().SetTimer(SpawnDummyDelayHandle, this, &AGOCLEANCharacter::SpawnDummyCharacter, AnimationDuration - 1.5f, false);
	GetWorldTimerManager().SetTimer(DestroyHandle, [SpawnedDummyActor]() { SpawnedDummyActor->Destroy(); }, AnimationDuration + 0.5f, false);
	
	if (StatsComp->GetCurrentLife() <= 0) return;
}

void AGOCLEANCharacter::Respawn()
{
	Server_RequestSetVisible(true);
	FirstPersonMeshComp->SetHiddenInGame(false);
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetIgnoreMoveInput(false);
	CameraComp->bUsePawnControlRotation = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	SetActorLocationAndRotation(RespawnTransform.GetLocation(), RespawnTransform.GetRotation());

	StatsComp->DecreaseLife(1);
	StatsComp->ResetStats();
}

void AGOCLEANCharacter::PlayHuntCameraSequence()
{
	if (HuntCameraSequence == nullptr) return;
	FirstPersonMeshComp->SetHiddenInGame(true);

	if (AnimState == EPlayerAnimState::Crouch) AGOCLEANCharacter::TryCrouch();

	ALevelSequenceActor* SequenceActor = nullptr;
	
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		HuntCameraSequence,
		FMovieSceneSequencePlaybackSettings(),
		SequenceActor
	);

	SequenceActor->bOverrideInstanceData = true;

	UDefaultLevelSequenceInstanceData* InstanceData = NewObject<UDefaultLevelSequenceInstanceData>(SequenceActor);
	InstanceData->TransformOrigin = FTransform(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z-100.0f));

	SequenceActor->DefaultInstanceData = InstanceData;

	SequenceActor->SetBindingByTag("PlayerCamera", { this });

	SequencePlayer->Play();
	

	SequencePlayer->OnFinished.AddDynamic(this, &AGOCLEANCharacter::Respawn);
}

void AGOCLEANCharacter::SpawnDummyCharacter()
{
	Server_RequestSetVisible(false);
	GetWorld()->SpawnActor<AActor>(DummyCharacter, GetActorLocation(), GetActorRotation())->SetOwner(this);
}

void AGOCLEANCharacter::Jump()
{

	Super::Jump();
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

	if (AnimState == EPlayerAnimState::Crouch)
	{
		GetCharacterMovement()->UCharacterMovementComponent::UnCrouch();

		CameraComp->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetWalkSpeed();

		SetAnimState(EPlayerAnimState::Stand);
	}
	else if(AnimState == EPlayerAnimState::Stand)
	{
		GetCharacterMovement()->UCharacterMovementComponent::Crouch();

		CameraComp->SetRelativeLocation(FVector(-10.0f, 0.0f, 30.0f));
		GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetCrouchSpeed();

		SetAnimState(EPlayerAnimState::Crouch);
	}
}

void AGOCLEANCharacter::Sprint()
{
	if (StatsComp == nullptr) return;

	if (AnimState == EPlayerAnimState::Crouch) return;

	if (StatsComp->GetCurrentStamina() <= 0.0f) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = StatsComp->GetSprintSpeed();

	bIsRecoveringStamina = false;
	GetWorldTimerManager().ClearTimer(StaminaRecoveryHandle);
}

void AGOCLEANCharacter::SprintRelease()
{
	if (StatsComp == nullptr) return;

	if (AnimState == EPlayerAnimState::Crouch) return;

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

// Animation //
void AGOCLEANCharacter::PlayerInteractionAnim()
{
	if (FirstPersonAnimDataTable == nullptr || ThirdPersonManAnimDataTable == nullptr || ThirdPersonWomanAnimDataTable == nullptr) return;
	
	const FAnimationData* FirstPersonAnimRow = FirstPersonAnimDataTable->FindRow<FAnimationData>(FName(*FString::FromInt(GetAnimID())), TEXT(""));

	if (FirstPersonAnimRow == nullptr) return;

	if (Gender == 0)
	{
		const FAnimationData* ThirdPersonAnimRow = ThirdPersonManAnimDataTable->FindRow<FAnimationData>(FName(*FString::FromInt(GetAnimID())), TEXT(""));

		if (FirstPersonAnimRow->Montage == nullptr || ThirdPersonAnimRow == nullptr) return;
		FirstPersonMeshComp->GetAnimInstance()->Montage_Play(FirstPersonAnimRow->Montage);
		ThirdPersonMeshComp->GetAnimInstance()->Montage_Play(ThirdPersonAnimRow->Montage);
	}
	else if (Gender == 1)
	{
		const FAnimationData* ThirdPersonAnimRow = ThirdPersonWomanAnimDataTable->FindRow<FAnimationData>(FName(*FString::FromInt(GetAnimID())), TEXT(""));

		if (FirstPersonAnimRow->Montage == nullptr || ThirdPersonAnimRow == nullptr) return;
		FirstPersonMeshComp->GetAnimInstance()->Montage_Play(FirstPersonAnimRow->Montage);
		ThirdPersonMeshComp->GetAnimInstance()->Montage_Play(ThirdPersonAnimRow->Montage);
	}
}

// Equipments & Interaction //
void AGOCLEANCharacter::DoInteraction()
{
	if (!InteractionComp || !EquipComp) return;

	// get equip id
	FName EquipID = EquipComp->GetCurrentEquipmentID();


	// change anim id to active-anim
	UGDataManagerSubsystem* DataManager = GetGameInstance()->GetSubsystem<UGDataManagerSubsystem>();
	const FGEquipmentDataRow* Data = DataManager->GetEquipmentData(EquipID);
	if (!Data) return;

	SetAnimID(Data->IdleToActivateAnimID_First);
	// require rpc function
	//		empty
}

void AGOCLEANCharacter::Server_TryInteraction(FName EquipID)
{
	UActorComponent* Target = InteractionComp->GetCurrentTarget();
	if (Target)
	{
		IGInteractable* Interactable = Cast<IGInteractable>(Target);
		if (Interactable)
		{
			if (Interactable->CanInteract(EquipID, Cast<AGOCLEANCharacter>(Target)))
			{
				Interactable->ExecuteInteraction(EquipID, Cast<AGOCLEANCharacter>(Target));

				UE_LOG(LogTemp, Log, TEXT("[GCharacter] Interaction Executed on %s"), *Target->GetOwner()->GetName());
			}
		}
	}
}

void AGOCLEANCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGOCLEANCharacter, EquipComp);
	DOREPLIFETIME(AGOCLEANCharacter, AnimID);
	DOREPLIFETIME(AGOCLEANCharacter, AnimState);
	DOREPLIFETIME(AGOCLEANCharacter, StatsComp);
}

