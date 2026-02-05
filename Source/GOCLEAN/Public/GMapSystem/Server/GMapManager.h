// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "GTypes/GMapTypes.h"

#include "GMapManager.generated.h"


class AGDoorway;


UCLASS()
class GOCLEAN_API UGMapManager : public UWorldSubsystem
{
	GENERATED_BODY()



	// constructor
public:
	UGMapManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


	
protected:
	UPROPERTY()
	TMap<FName, FGZoneData> ZoneDatas;

	UPROPERTY()
	TArray<TObjectPtr<AGDoorway>> Doorways;

};
