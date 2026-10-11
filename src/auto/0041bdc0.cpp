#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, InitRatioVec4, 0x0040A5E0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(void*, __fastcall, Daf_FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(float*, __fastcall, Wheel_ApplySetting, 0x0040B550, (void* self, void* edx, int a2, float* a3));

static int __fastcall C_0082e2e8_ApplyWheelRatioAndClearFlag(C_0082e2e8* self, void* /*edx*/, int a2)
{
    float steerRatio = -1.4835298f;
    int wheelKey;

    if (a2 == 0) {
        wheelKey = 5;
    } else {
        if (a2 != 1) {
            return a2 - 1;
        }
        wheelKey = 6;
        steerRatio = 1.4835298f;
    }

    float ratioVec[4];
    std::memset(ratioVec, 0, 12);
    ratioVec[3] = -1.0f;
    InitRatioVec4(ratioVec, nullptr, steerRatio, 0.4f);

    void* wheelEntry = Daf_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, wheelKey);
    Wheel_ApplySetting(wheelEntry, nullptr, 1, ratioVec);

    uint32_t wheelTable = self->field_0010;
    reinterpret_cast<uint8_t*>(wheelTable)[a2 * 24 + 12] = 0;
    return static_cast<int>(wheelTable);
}

HOOK(0x0041BDC0, C_0082e2e8_ApplyWheelRatioAndClearFlag, "auto");
