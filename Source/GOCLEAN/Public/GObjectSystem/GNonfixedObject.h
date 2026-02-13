// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GTypes/GObjectTypes.h"
#include "GTypes/IGInteractable.h"

#include "GNonfixedObject.generated.h"


class UStaticMeshComponent;
class UGAdditionalObjFuncComponent;
class UBoxComponent;


DECLARE_MULTICAST_DELEGATE(FOnNonfixedObjIneracted);


UCLASS()
class GOCLEAN_API AGNonfixedObject : public AActor, public IGInteractable
{
	GENERATED_BODY()
	


	// variables
private:

	// Variables //
	// instance ID - ObjectPool에서 재활용 시, 생성된 obj cnt가 instance id로 사용됨
	UPROPERTY(VisibleAnywhere)
	int32 IID;

	// type ID - 현재 오브젝트가 어떤 오브젝트인지 식별하는 ID
	UPROPERTY(VisibleAnywhere)
	FName TID;

	// static mesh component - TID에 따른 오브젝트 static mesh 
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;


	// state - 현재 오브젝트의 상태
	UPROPERTY(VisibleAnywhere)
	ENonfixedObjState State;

	
	// Actions //
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> InteractionVolume; // Collision Profile -> Interactable

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UGAdditionalObjFuncComponent>> FuncCompList;

	UPROPERTY(VisibleAnywhere)
	int32 InteractionCnt;

	// 인터랙션 발생 시, 해당 오브젝트가 수행해야할 동작 묶음
	FOnNonfixedObjIneracted OnNonfixedObjInteracted;


	// custom func
public:
	// initialize
	void CleanUp();
	bool Initialize(const FGNonfixedObjData& NewData);

	// state
	bool ChangeState(ENonfixedObjState ChangedState);

	// interact
	virtual bool CanInteract(FName EquipID) const override;
	virtual void ExecuteInteraction(FName EquipID) override;

	FOnNonfixedObjIneracted& GetOnInteractionDelegate() { return OnNonfixedObjInteracted; }


private:
	// state
	void OnStateExit();
	void OnStateEnter();
	void OnStateUpdate();

	// interact
	void TriggerInteraction()
	{
		OnNonfixedObjInteracted.Broadcast();
	}



	// constructor
public:	
	AGNonfixedObject();



	// life cycle
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
