#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "GMapDataRow.generated.h"


USTRUCT(BlueprintType)
struct FGMapDataRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FGMapDataRow() {}


    // Shared //
    //      ID of the zone included in the current map
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shared")
    FName ZoneID;


    // Visual //


    // Logic //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    TArray<FName> SpawnableObjsID;

};
