// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "GEquipmentComponent.generated.h"


class AGOCLEANCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOCLEAN_API UGEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()



	// Variables
private:
	// only host



	// replicated
	//		slot datas
	UPROPERTY(Replicated)	
	int32 CurrentSlotIndex;

	UPROPERTY(Replicated)
	TArray<FName> EquipmentSlots; // ¿Â∫Ò ΩΩ∑‘(size 4), element: equip ID


	//		pollution
	UPROPERTY(Replicated)
	float MopPollution;

	UPROPERTY(Replicated)
	float AutoMopPollution;
	

	//		owner character
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AGOCLEANCharacter> Onwer;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USkeletalMeshComponent>> SlotVisualComponents;



	// functions - custom
public:
	// in-game
	FName GetCurrentEquipmentID();
	bool ChangeEuquipmentInCurrSlot(FName ChangedEquipID);
	bool ChangeCurrentSlot(int32 ChangedSlotIndex);


private:
	// in-game
	FName GetEquipmentID(int32 SlotIndex);
	bool ChangeEquipment(int32 SlotIndex, FName EquipID);
	bool ChangeCurrentSlot_Interval(int32 From, int32 To);

	// in-construct 

	// in-beginPlay
	bool InitiateEquipmentSlots();
	bool RegisterSlotVisualComps();
	



	// functions - default
public:	
	UGEquipmentComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
