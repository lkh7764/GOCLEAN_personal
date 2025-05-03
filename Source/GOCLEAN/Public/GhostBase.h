// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GhostBase.generated.h"

UCLASS(Abstract)
class GOCLEAN_API AGhostBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGhostBase();

	virtual void StartEnragedChase(AActor* Target) PURE_VIRTUAL(AGhostBase::StartEnragedChase, );

	virtual void PlayCommonSound() PURE_VIRTUAL(AGhostBase::PlayCommonSound, );

	virtual void Manifest() PURE_VIRTUAL(AGhostBase::Manifest, );

	virtual void Patrol() {};

protected:
	virtual void BeginPlay() override;

	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Ghost|Audio");
	USoundBase* CommonSound;

	UPROPERTY(VisibleAnywhere, Category = "Ghost|State");
	bool bIsEnraged;

	UPROPERTY(VisibleAnywhere, Category = "Ghost|State");
	AActor* TargetActor;

public:
	// 없애도 되지 않나?
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
