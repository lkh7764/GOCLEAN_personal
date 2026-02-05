// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"



UENUM(BlueprintType)
enum class ENonfixedObjStatus : uint8 {
	E_Static			UMETA(DisplayName = "Static - 정적"),
	E_Kinematic			UMETA(DisplayName = "Kinematic - 운동중"),
	E_Invisible			UMETA(DisplayName = "Invisible - 비가시적"),
	E_Disintegrating	UMETA(DisplayName = "Disintegrating - 소각 중"),
	E_Destroyed			UMETA(DisplayName = "Destroyed - 파괴됨"),

	E_None				UMETA(DisplayName = "None - 오브젝트 할당되지 않음"),

	E_MAX			UMETA(Hidden)
};



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
