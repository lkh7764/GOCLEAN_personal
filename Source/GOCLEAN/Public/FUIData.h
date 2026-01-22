// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FUIData.generated.h"

class UGUIBase;

UENUM(BlueprintType)
enum class EUIType : uint8 { Layer, Popup };

USTRUCT(BlueprintType)
struct GOCLEAN_API FUIData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EUIType UIType = EUIType::Layer;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGUIBase> WidgetBluePrintClass = nullptr;

	UPROPERTY(EditAnywhere)
	int32 ZOrder = 0;
};