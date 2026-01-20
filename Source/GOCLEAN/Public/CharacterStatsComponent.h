// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterStatsComponent();


	// Getter, Setter //
	float GetCurrentSanity() const;

	int GetCurrentLife() const;
	void DecreaseLife(int32 Amount);

	void SetCurrentSanity(float Amount);
	void IncreaseCurrentSanity(float Amount);
	void DecreaseCurrentSanity(float Amount);

	float GetCurrentStamina() const;
	float GetStaminaDrainRate() const;
	float GetStaminaRecoveryRate() const;
	float GetStaminaRecoveryDelay() const;
	float GetMaxStamina() const;
	void SetCurrentStamina(float NewCurrentStamina);
	void IncreaseCurrentStamina(float Amount);
	void DecreaseCurrentStamina(float Amount);

	float GetWalkSpeed() const;
	float GetCrouchSpeed() const;
	float GetSprintSpeed() const;
	void SetDefaultSpeed(float NewDefaultSpeed);
	void IncreaseDefaultSpeed(float Amount);
	void DecreaseDefaultSpeed(float Amount);


private:
	// Overidded //
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Initialize //
	void InitializeStats(
		int32 BaseLife, 
		int32 BaseMaxSanity, 
		float BaseMaxStamina, float BaseStaminaDrainRate, float BaseStaminaRecoveryRate, float BaseStaminaRecoveryDelay,
		float BaseDefaultSpeed)
	{
		Life = BaseLife;

		MaxSanity = BaseMaxSanity;
		CurrentSanity = MaxSanity;

		MaxStamina = BaseMaxStamina;
		StaminaDrainRate = BaseStaminaDrainRate;
		StaminaRecoveryRate = BaseStaminaRecoveryRate;
		StaminaRecoveryDelay = BaseStaminaRecoveryDelay;

		DefaultSpeed = BaseDefaultSpeed;
		WalkSpeed = DefaultSpeed;
		CrouchSpeed = DefaultSpeed - 300.0f;
		SprintSpeed = DefaultSpeed + 300.0f;
	}


	// Character stats //
	int32 Life;

	float MaxSanity;
	float CurrentSanity;

	float MaxStamina;
	float CurrentStamina;
	float StaminaDrainRate;
	float StaminaRecoveryRate;
	float StaminaRecoveryDelay;

	float DefaultSpeed;
	float WalkSpeed;
	float CrouchSpeed;
	float SprintSpeed;
};
