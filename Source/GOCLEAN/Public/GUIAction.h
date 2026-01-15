// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GUIAction.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew) // DefaultToInstanced
class GOCLEAN_API UGUIAction : public UObject
{
	GENERATED_BODY()
	


// Funcitons //
// JSH MEMO: Change access specifier public to private & declare friend class with GUIManager?
public:
	virtual void ExecuteAction(UUserWidget* Target) PURE_VIRTUAL(UUIAction::ExecuteAction, );

	virtual bool IsFinished() const { return true; };
};