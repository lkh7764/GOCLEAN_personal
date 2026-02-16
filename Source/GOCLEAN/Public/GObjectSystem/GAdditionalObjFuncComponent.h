// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GTypes/GObjectTypes.h"

#include "GAdditionalObjFuncComponent.generated.h"


class AGOCLEANCharacter;
class AGNonfixedObject;


UCLASS(Abstract, Blueprintable)
class GOCLEAN_API UGAdditionalObjFuncComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	UGAdditionalObjFuncComponent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) {}

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) PURE_VIRTUAL(UGAdditionalObjFuncComponent::OnInteractionTriggered, );

	UFUNCTION()
	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) 
		PURE_VIRTUAL(UGAdditionalObjFuncComponent::OnStateChangeTriggered, );
		
};


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GOCLEAN_API UGPickComponent : public UGAdditionalObjFuncComponent
{
	GENERATED_BODY()


public:
	UGPickComponent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) override;

	UFUNCTION()
	void DropObject();


protected:
	virtual void BeginPlay() override;

	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) override;

	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) override;


	// custom functions
	UFUNCTION()
	void PickUpObject(AGOCLEANCharacter* Target);


private:
	UPROPERTY(VisibleAnywhere)
	bool bIsPickedUp = false;

	UPROPERTY(VisibleAnywhere)
	AGOCLEANCharacter* OwnerPlayer;

};


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GOCLEAN_API UGRemovingComponent : public UGAdditionalObjFuncComponent
{
	GENERATED_BODY()


public:
	UGRemovingComponent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) override;


protected:
	virtual void BeginPlay() override;

	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) override;

	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) override {};


	// custom functions
	UFUNCTION()
	void SetVisualByInteractionCnt(AGNonfixedObject* Owner);

	UFUNCTION()
	void SetDestroyThisObject(AGNonfixedObject* Owner);


private:
	int32 InteractionMaxCnt;

};


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GOCLEAN_API UGMultiInteractionComponent : public UGAdditionalObjFuncComponent
{
	GENERATED_BODY()


public:
	UGMultiInteractionComponent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) override;


protected:
	virtual void BeginPlay() override;

	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) override {};

	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) override {};

};


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GOCLEAN_API UGBurningCompopnent : public UGAdditionalObjFuncComponent
{
	GENERATED_BODY()


public:
	UGBurningCompopnent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) override;


protected:
	virtual void BeginPlay() override;

	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) override {};

	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) override;


	// custom functions
	UFUNCTION()
	void StartBurning();


private:
	FTimerHandle BurnTimerHandle;

	void OnBurnTimerFinished();

	UPROPERTY(EditAnywhere, Category = "Burning")
	float BurningInterval = 5.0f;

};



UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GOCLEAN_API UGSpawnerCompopnent : public UGAdditionalObjFuncComponent
{
	GENERATED_BODY()


public:
	UGSpawnerCompopnent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) override;


protected:
	virtual void BeginPlay() override;

	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) override {};

	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) override;


	// custom functions
	UFUNCTION()
	void SpawnDerivedObject(AGNonfixedObject* Owner);

};


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class GOCLEAN_API UGInteractSoundCompopnent : public UGAdditionalObjFuncComponent
{
	GENERATED_BODY()


public:
	UGInteractSoundCompopnent();

	virtual void InitializeAdditionalData(const FGNonfixedObjData& Data) override;


protected:
	virtual void BeginPlay() override;

	virtual void OnInteractionTriggered(AGOCLEANCharacter* Target) override;

	virtual void OnStateChangeTriggered(ENonfixedObjState PrevState, ENonfixedObjState ChangedState) override {};


	// custom functions
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USoundBase> CachedInteractSound;

};
