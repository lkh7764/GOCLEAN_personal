#include "CharacterStatsComponent.h"

UCharacterStatsComponent::UCharacterStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Character base stats initalize
	InitializeStats(2, 100.0f, 10.0f, 1.0f, 1.0f, 1.0f, 600.0f);
}

void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


// Getter, Setter //
void UCharacterStatsComponent::DecreaseLife(int32 Amount) { Life -= Amount; }

float UCharacterStatsComponent::GetCurrentSanity() const { return CurrentSanity; }
void UCharacterStatsComponent::SetCurrentSanity(float NewCurrentSanity) { CurrentSanity = NewCurrentSanity; }
void UCharacterStatsComponent::IncreaseCurrentSanity(float Amount) { CurrentSanity += Amount; }
void UCharacterStatsComponent::DecreaseCurrentSanity(float Amount) { CurrentSanity -= Amount; }

float UCharacterStatsComponent::GetCurrentStamina() const { return CurrentStamina; }
float UCharacterStatsComponent::GetStaminaDrainRate() const { return StaminaDrainRate; }
float UCharacterStatsComponent::GetStaminaRecoveryRate() const { return StaminaRecoveryRate; }
float UCharacterStatsComponent::GetStaminaRecoveryDelay() const { return StaminaRecoveryDelay; }
float UCharacterStatsComponent::GetMaxStamina() const { return MaxStamina;  }
void UCharacterStatsComponent::SetCurrentStamina(float NewCurrentStamina) { CurrentStamina = NewCurrentStamina; }
void UCharacterStatsComponent::IncreaseCurrentStamina(float Amount) { CurrentStamina += Amount; }
void UCharacterStatsComponent::DecreaseCurrentStamina(float Amount) { CurrentStamina -= Amount; }

float UCharacterStatsComponent::GetWalkSpeed() const { return WalkSpeed; }
float UCharacterStatsComponent::GetCrouchSpeed() const { return CrouchSpeed; }
float UCharacterStatsComponent::GetSprintSpeed() const { return SprintSpeed; }
void UCharacterStatsComponent::IncreaseDefaultSpeed(float Amount) { DefaultSpeed += Amount; }
void UCharacterStatsComponent::DecreaseDefaultSpeed(float Amount) { DefaultSpeed -= Amount; }
void UCharacterStatsComponent::SetDefaultSpeed(float NewDefaultSpeed) {
	DefaultSpeed = NewDefaultSpeed;
	WalkSpeed = NewDefaultSpeed;
	CrouchSpeed = NewDefaultSpeed - 300;
	SprintSpeed = NewDefaultSpeed + 300;
}