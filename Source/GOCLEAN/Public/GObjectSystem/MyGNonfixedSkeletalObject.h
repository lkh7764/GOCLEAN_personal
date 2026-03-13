// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MyGNonfixedSkeletalObject.generated.h"


class UGAdditionalObjFuncComponent;
class UGNonfixedObjCoreComponent;
class UBoxComponent;
class USkeletalMeshComponent;


UCLASS()
class GOCLEAN_API AGNonfixedSkeletalObject : public AActor
{
	GENERATED_BODY()


	// defaults
public:
	AGNonfixedSkeletalObject();

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// life cycle
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	// variables
protected:
	// root
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> RootPrimitive;

	void UpdateInteractionBounds();

	// interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> InteractionVolume; // Collision Profile -> Interactable


	// root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UGNonfixedObjCoreComponent> CoreComp;

};
