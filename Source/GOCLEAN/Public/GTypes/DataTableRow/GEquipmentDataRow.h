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


    // Visual //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UStaticMesh> MeshAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FVector RelativeLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FRotator RelativeRotation;


    // Logic //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    float Power;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    int32 MatchedSlotIndex;

};
