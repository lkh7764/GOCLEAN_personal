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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID")
    int32 IdleAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID")
    int32 IdleToActivateAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID")
    int32 ActivateToIdleAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID")
    int32 ActivateAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID")
    int32 SwapAnimID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Mesh")
    TSoftObjectPtr<UStaticMesh> SM_MeshAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Mesh")
    TSoftObjectPtr<USkeletalMesh> SK_MeshAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Socket")
    FVector RelativeLocation_ManThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Socket")
    FVector RelativeLocation_WomanThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Socket")
    FVector RelativeLocation_FirstPerson;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Socket")
    FRotator RelativeRotation_ManThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Socket")
    FRotator RelativeRotation_WomanThird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|Socket")
    FRotator RelativeRotation_FirstPerson;


    // Logic //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    float Power;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    int32 MatchedSlotIndex;

};
