#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));

// Stores the raw dword argument into the float field at +0x40, then clamps that field
// in place to the range [0.0f, 1.0f].
static void __fastcall C_008673b0_SetClampedUnitValue(C_008673b0* self, void* /*edx*/, int rawValueBits)
{
    std::memcpy(&self->field_0040, &rawValueBits, sizeof(float));

    float lowerBound = 0.0f;
    float upperBound = 1.0f;
    ClampFloatToBounds(&self->field_0040, &lowerBound, &upperBound);
}

HOOK(0x007390D0, C_008673b0_SetClampedUnitValue, "auto");
