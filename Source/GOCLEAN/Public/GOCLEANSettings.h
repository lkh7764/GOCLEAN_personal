

#pragma once

#include "Engine/DeveloperSettings.h"
#include "GOCLEANSettings.generated.h"


UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "GoClean Project Settings"))
class GOCLEAN_API UGOCLEANSettings : public UDeveloperSettings
{
    GENERATED_BODY()


public:
    // UGOCLEANSettings();

    UPROPERTY(Config, EditAnywhere, Category = "Participant", meta = (ClampMin = "1", ClampMax = "10"))
    int32 MaxParticipant;

    UPROPERTY(Config, EditAnywhere, Category="Equipment", meta=(ClampMin = "1", ClampMax = "10"))
    int32 MaxEquipmentSlots;

    static const UGOCLEANSettings* Get() { return GetDefault<UGOCLEANSettings>(); }
};
