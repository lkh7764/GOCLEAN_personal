// Fill out your copyright notice in the Description page of Project Settings.


#include "BurntOutGhost.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

ABurntOutGhost::ABurntOutGhost()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}



void ABurntOutGhost::BeginPlay() 
{
	Super::BeginPlay();
}

void ABurntOutGhost::StartEnragedChase(AActor* Target)
{
	bIsEnraged = true;
	TargetActor = Target;

	/*
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && Target)
	{
		AIController->MoveToActor(Target, 100.0f);
		UE_LOG(LogTemp, Log, TEXT("BurntOutGhost chase"));
	}
	*/
}

void ABurntOutGhost::PlayCommonSound() {
	if (CommonSound == nullptr) return;
	
	UGameplayStatics::PlaySoundAtLocation(this, CommonSound, GetActorLocation());
}

void ABurntOutGhost::Manifest()
{
	Mesh->SetVisibility(true);
}