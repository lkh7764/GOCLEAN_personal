// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostBase.h"

// Sets default values
AGhostBase::AGhostBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGhostBase::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh = GetMesh();
	//Mesh->SetVisibility(false); 시작할 때 안 보이게
}

// Called every frame
void AGhostBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGhostBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

