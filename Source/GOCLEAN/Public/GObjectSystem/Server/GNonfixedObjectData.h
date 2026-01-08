// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "GTypes/GObjectTypes.h"

#include "GNonfixedObjectData.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UGNonfixedObjectData : public UActorComponent
{
	GENERATED_BODY()



	// constructor
public:	
	UGNonfixedObjectData();



	// variables
protected:
	UPROPERTY(VisibleAnywhere)
	int32 _objectID;

	UPROPERTY(VisibleAnywhere)
	int32 _objectType;

	UPROPERTY(VisibleAnywhere)
	ENonfixedObjStatus _currStatus;

	UPROPERTY(VisibleAnywhere)
	FVector _location;



	// functions
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
