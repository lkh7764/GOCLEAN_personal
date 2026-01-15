// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GUIBase.generated.h"

UCLASS(Abstract)
class GOCLEAN_API UGUIBase : public UUserWidget
{
	GENERATED_BODY()



// Funcitons //
protected:
	UPROPERTY(EditDefaultsOnly)
	FName UIID;

public:
	virtual void OnSetup();

// Getter, Setter
public:
	FName GetUIID() const { return UIID; };
};