
#include "GOCLEANSettings.h"


#if WITH_EDITOR
void UGOCLEANSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    PollutionThreshold_LV0 = PollutionMax * 0.0f;
    PollutionThreshold_LV1 = PollutionMax * 0.33f;
    PollutionThreshold_LV2 = PollutionMax * 0.66f;
    PollutionThreshold_LV3 = PollutionMax * 1.0f;
}
#endif
