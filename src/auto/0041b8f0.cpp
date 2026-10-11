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

// If the incoming pointer is null, builds the default wheel ratio vector {0,0,0,-1},
// initializes it with (1.2217305, 0.4), applies it through the data object found for key 7,
// and clears the flag byte at this->field_0010 + 12.
static float* __fastcall C_0082e280_InitDefaultWheelRatioIfNull(C_0082e280* self, void* /*edx*/, float* a2)
{
    float* result = a2;
    if (!a2)
    {
        float defaultRatio[4] = { 0.0f, 0.0f, 0.0f, -1.0f };
        RatioVec4_Init(defaultRatio, nullptr, 1.2217305f, 0.4f);

        auto* dataObj = reinterpret_cast<C_0082daf4*>(
            Daf4_FindOrCreateByKey(reinterpret_cast<void*>(self->field_0030), nullptr, 7));
        result = Wheel_ApplyRatio(dataObj, nullptr, 1, defaultRatio);

        reinterpret_cast<uint8_t*>(self->field_0010)[12] = 0;
    }
    return result;
}

HOOK(0x0041B8F0, C_0082e280_InitDefaultWheelRatioIfNull, "auto");
