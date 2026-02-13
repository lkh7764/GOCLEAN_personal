#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "GEquipmentDataRow.generated.h"


USTRUCT(BlueprintType)
struct FGEquipmentDataRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FGEquipmentDataRow() {}


    // Shared //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared")
    FName EquipID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared")
    FText DisplayName;


    // Visual //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    int32 IdleAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    int32 ActivateAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    int32 DisableAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    int32 InteractionAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UStaticMesh> MeshAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FVector RelativeLocation_ManThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FVector RelativeLocation_WomanThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FVector RelativeLocation_FirstPerson;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FRotator RelativeRotation_ManThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FRotator RelativeRotation_WomanThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FRotator RelativeRotation_FirstPerson;


    // Logic //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    float Power;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    int32 MatchedSlotIndex;

};
