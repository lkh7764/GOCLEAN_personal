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



	// variables - custom
private:
	UPROPERTY(Replicated)	// 현재 선택한 슬롯의 EquipmentSlots 상 index
	int32 CurrentSlotIndex;

	UPROPERTY(Replicated)	// 장비 슬롯(size 4), element: equip ID
	TArray<FName> EquipmentSlots;


	// in-owner character
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
	// Sets default values for this component's properties
	UGEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
