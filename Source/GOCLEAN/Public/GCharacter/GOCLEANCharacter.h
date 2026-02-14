/**
 * @class AGOCLEANCharacter
 * @brief Character stats component를 기반으로 하여, 유저의 입력에 기반한 플레이어 이동 처리
 *
 * **[Core functions]**
 * - Move(const FInputActionValue& Value): 키 입력에 따른 플레이어 이동 처리
 * - Look(const FInputActionValue& Value): 마우스 이동에 따른 플레이어 카메라 이동
 * - Crouch(): 플레이어 웅크리기 상태 전환
 * - Sprint(): 플레이어 달리기
 * - SprintRelease(): 플레이어 달리기 상태 릴리즈
 * - StartStaminaRecovery(): Stamina 재생 시작
 * - RecoverStamina(): Stamina 재생
 * - ToggleFlashlight(): 플레이어 손전등 토글 기능
 * 
 * **[Getter & Setter functions]**
 * - IsCrouching() const: 웅크리기 상태인지에 대한 판단값 반환
 * - IsSprinting() const: 달리기 상태인지에 대한 판단값 반환
 * - GetPlayerCurrentSanity() const: 플레이어의 현재 정신력 반환
 * - SetPlayerCurrentSanity(float NewPlayerCurrentSanity): 플레이어의 현재 정신력 설정
 * - OnHunted(): Ghost에게 Hunt 당했을 시 호출
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "InputAction.h"

#include "GCharacter/StatsComponent/CharacterStatsComponent.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "DefaultLevelSequenceInstanceData.h"

#include "GOCLEANCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpotLightComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class UGEquipmentComponent;
class UInteractionComponent;
class UEnhancedInputComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class GOCLEAN_API AGOCLEANCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGOCLEANCharacter();


	// Getter, Setter //
	UFUNCTION(BlueprintPure)
	bool IsCrouching() const { return bIsCrouching; }
	UFUNCTION(BlueprintPure)
	bool IsSprinting() const { return bIsSprinting; }

	float GetPlayerCurrentSanity() const;
	void SetPlayerCurrentSanity(float NewPlayerCurrentSanity);


	// OnHunted //
	void OnHunted();

	void PlayHuntCameraSequence();


private:
	// Components //
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> FirstPersonMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ThirdPersonMeshComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpotLightComponent> FlashlightComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCharacterStatsComponent> StatsComp;


	// Dummy //
	UPROPERTY(EditAnywhere, Category = "DummyActor")
	TSubclassOf<AActor> DummyGhost;

	UPROPERTY(EditAnywhere, Category = "DummyActor")
	TSubclassOf<AActor> DummyCharacter;


	// Respawn //
	UPROPERTY(EditAnywhere, Category = "RespawnPoint")
	FTransform RespawnTransform;


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


	// Level sequence //
	UPROPERTY(EditAnywhere, Category="Camera sequence")
	TObjectPtr<ULevelSequence> HuntCameraSequence;


	// Respawn //
	UFUNCTION()
	void Respawn();



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


	// Equipments & Interaction //
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 AnimID;

	UFUNCTION(BlueprintCallable)
	void SetAnimID(int32 NewID) { AnimID = NewID; }

	UFUNCTION(BlueprintPure)
	int32 GetAnimID() { return AnimID; }


	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<UGEquipmentComponent> EquipComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> ChangeSlotAction;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInteractionComponent> InteractionComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input Actions")
	TObjectPtr<UInputAction> InteractAction;


public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void DoInteraction();

	// only Server
	void Server_TryInteraction(FName EquipID);
	
};
