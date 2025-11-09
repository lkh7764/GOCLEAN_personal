// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"



UENUM(BlueprintType)
enum class EFixedObjStatus : uint8 {
	E_Static			UMETA(DisplayName = "Static - Á¤Àû"),
	E_Used				UMETA(DisplayName = "Used - »ç¿ëÁß"),
	E_Destroyed			UMETA(DisplayName = "Destroyed - ÆÄ±«µÊ"),

	E_MAX				UMETA(Hidden)
};



#include "GFixedObjectData.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UGFixedObjectData : public UActorComponent
{
	GENERATED_BODY()


	// constructor
public:	
	UGFixedObjectData();



	// variables
	UPROPERTY(VisibleAnywhere)
	int32 objectType;

	UPROPERTY(VisibleAnywhere)
	EFixedObjStatus currStatus;

	UPROPERTY(VisibleAnywhere)
	FVector location;

	UPROPERTY(VisibleAnywhere)
	int32 maxUserNum;

	UPROPERTY(VisibleAnywhere)
	int32 userCnt;



	// functions
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
