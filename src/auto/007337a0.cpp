#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __fastcall S_007337a0_SumMatchingEntryValues(
    S_007337a0* self, void* /*edx*/, int key)
{
    double result = g_fZero;
    float sum = g_fZero;
    const uint32_t count = self->field_0070;
    uint32_t offset = 0;
    bool found = false;

    for (uint32_t i = 0; i < count; ++i, offset += 48)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0068) + offset;
        if (CSF_AT(int, entry, 0x2C) == key)
        {
            found = true;
            int value = GetIndexedTableValue_6AF8A0(
                reinterpret_cast<void*>(self->field_0008), nullptr,
                CSF_AT(int, entry, 0x04));
            if (value)
            {
                void* object = reinterpret_cast<void*>(value);
                float* component =
                    ((float* (__fastcall*)(void*, void*))
                        CSF_AT(void**, object, 0)[0x40 / 4])(object, nullptr);
                result = *component + sum;
            }
            else
            {
                result = g_fZero + sum;
            }
            sum = static_cast<float>(result);
        }
        else if (found)
        {
            return result;
        }
    }
    return result;
}

HOOK(0x007337A0, S_007337a0_SumMatchingEntryValues, "auto");
