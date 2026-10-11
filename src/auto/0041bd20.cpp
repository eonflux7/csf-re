#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0,
         (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, Vehicle_InitRatioVec4, 0x0040A5E0,
         (void* self, void* edx, float a2, float a3));
CSF_FUNC(float*, __fastcall, Vehicle_SetVectorProperty, 0x0040B550,
         (void* self, void* edx, int a2, float* a3));

static int __fastcall C_0082e2e8_InitializeWheelConfiguration(
    C_0082e2e8* self, void* /*edx*/, int wheelIndex)
{
    float ratio = 1.0f;
    float angle = 1.4835298f;
    int key;

    if (wheelIndex != 0)
    {
        if (wheelIndex != 1)
            return static_cast<int>(static_cast<uint32_t>(wheelIndex) - 1u);

        key = 6;
        angle = -1.4835298f;
        ratio = 0.40000001f;
    }
    else
    {
        key = 5;
    }

    float vector[6] = {0.0f, 0.0f, 0.0f, -1.0f, angle, ratio};
    Vehicle_InitRatioVec4(vector, nullptr, angle, ratio);

    void* property = C_0082daf4_FindOrCreateByKey(
        reinterpret_cast<void*>(self->field_0030), nullptr, key);
    Vehicle_SetVectorProperty(property, nullptr, 1, vector);

    CSF_AT(uint8_t,
           reinterpret_cast<uint8_t*>(self->field_0010) + 24 * wheelIndex,
           0x0C) = 1;

    return 3 * wheelIndex;
}

HOOK(0x0041BD20, C_0082e2e8_InitializeWheelConfiguration, "auto");
