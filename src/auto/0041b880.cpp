#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address, signatures per packet)
CSF_FUNC(void*, __fastcall, Daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, RatioVec4_Init, 0x0040A5E0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(float*, __fastcall, Wheel_ApplyRatio, 0x0040B550, (void* self, void* edx, int a2, float* a3));

static float* __fastcall C_0082e280_InitDefaultWheelRatio(C_0082e280* self, void* /*edx*/, float* a2)
{
    float* result = a2;
    if (!a2)
    {
        float defaultRatio[4] = { 0.0f, 0.0f, 0.0f, -1.0f };
        RatioVec4_Init(defaultRatio, nullptr, -1.2217305f, 1.4f);

        auto* dataObj = reinterpret_cast<C_0082daf4*>(
            Daf4_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 7));
        result = Wheel_ApplyRatio(dataObj, nullptr, 1, defaultRatio);

        *(reinterpret_cast<uint8_t*>(self->field_0010) + 12) = 1;
    }
    return result;
}

HOOK(0x0041B880, C_0082e280_InitDefaultWheelRatio, "auto");
