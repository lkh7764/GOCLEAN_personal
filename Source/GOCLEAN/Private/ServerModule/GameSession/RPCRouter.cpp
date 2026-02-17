// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerModule/GameSession/RPCRouter.h"

// Sets default values
ARPCRouter::ARPCRouter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPCRouter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPCRouter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

