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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|First")
    int32 IdleAnimID_First;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|First")
    int32 IdleToActivateAnimID_First;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|First")
    int32 ActivateToIdleAnimID_First;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|First")
    int32 ActivateAnimID_First;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|First")
    int32 SwapAnimID_First;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|Third")
    int32 IdleAnimID_Third;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|Third")
    int32 IdleToActivateAnimID_Third;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|Third")
    int32 ActivateToIdleAnimID_Third;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|Third")
    int32 ActivateAnimID_Third;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual|AnimID|Third")
    int32 SwapAnimID_Third;


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
