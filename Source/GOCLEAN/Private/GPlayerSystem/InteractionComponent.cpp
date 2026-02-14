// Fill out your copyright notice in the Description page of Project Settings.

#include "GPlayerSystem/InteractionComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GTypes/IGInteractable.h"
// #include "Debug/DebugDrawService.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PerformLineTrace();
}


void UInteractionComponent::PerformLineTrace()
{
	// effectiveness check
	AActor* Owner = GetOwner();
	if (!Owner) return;


	// get camera location and rotation
	FVector Start;
	FRotator Rotation;
	Owner->GetActorEyesViewPoint(Start, Rotation);

	FVector End = Start + (Rotation.Vector() * InteractionRange);


	// set raycast
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	
	// shoot ray
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_GInteractable, Params);
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 0.1f, 0, 2.0f);
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor) return;

		TArray<UActorComponent*> InteractComps =
			HitActor->GetComponentsByInterface(UGInteractable::StaticClass());

		if (InteractComps.Num() > 0)
		{
			CurrentTarget = InteractComps[0];
			return;
		}
	}

	CurrentTarget = nullptr;
}

