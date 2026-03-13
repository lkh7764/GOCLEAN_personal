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

	// -------------------------------
	//		IN-DOORWAY
	// -------------------------------
	void PlayerEnteredZone(FName ZoneName, AActor* PlayerActor);
	void PlayerExitedZone(FName ZoneName, AActor* PlayerActor);

	void GhostEnteredZone(FName ZoneName, AActor* GhostActor);
	void GhostExitedZone(FName ZoneName, AActor* GhostActor);

	const FGZoneData* GetZoneData(FName ZoneName) const;

	
protected:
	UPROPERTY()
	TMap<FName, FGZoneData> ZoneDatas;

};
