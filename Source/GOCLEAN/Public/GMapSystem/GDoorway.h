// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GDoorway.generated.h"



UCLASS()
class GOCLEAN_API AGDoorway : public AActor
{
	GENERATED_BODY()
	


	// Default & Replication
public:	
	AGDoorway();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;



	// Variables
protected:
	// only host
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UBoxComponent* TriggerBox;

	// replicated
	UPROPERTY(VisibleAnywhere, Replicated, Category = "Zone")
	FName FromZone;

	UPROPERTY(VisibleAnywhere, Replicated, Category = "Zone")
	FName ToZone;

	UPROPERTY(EditAnywhere, Replicated, Category = "Status")
	bool bCanClose;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_IsClosed, Category = "Status")
	bool bIsClosed;

	// door bp를 통해 문 회전을 보여줘야 함
	// 1안. door bp에 회전을 만들고, 해당 bp를 property로 등록시킨다
	//		- open 이벤트 발생 시, bp에 등록된 회전 기능을 실행한다.
	// 2안. skeletal mesh를 넣어준다. 
	//		- 이렇게 되면 door 재배치해야할 수도



	// Funcitons
private:
	// only server
	void EnterPlayer();
	void ExitPlayer();

	void EnterGhost();
	void ExitGhost();

protected:
	// Trigger
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// OnRep
	UFUNCTION()
	void OnRep_IsClosed();

};
