// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GNonfixedObject.generated.h"


class UGAdditionalObjFuncComponent;
class UGNonfixedObjCoreComponent;
class UBoxComponent;
class UStaticMeshComponent;


UCLASS()
class GOCLEAN_API AGNonfixedObject : public AActor
{
	GENERATED_BODY()


	// defaults
public:
	AGNonfixedObject();

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// life cycle
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UGNonfixedObjCoreComponent* GetNonfixedObjCoreComp() const { return CoreComp; }


	// variables
protected:
	// root
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RootPrimitive;

	void UpdateInteractionBounds();

	// interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> InteractionVolume; // Collision Profile -> Interactable


	// root
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UGNonfixedObjCoreComponent> CoreComp;

};
