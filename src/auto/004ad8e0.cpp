#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Stores the two 3-component vectors and a unit scale (1.0f) into the object.
static int __fastcall InitUnitScaleAndCopyTwoVec3(void* self, void* /*edx*/,
                                                  const int32_t* srcA, const int32_t* srcB)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    CSF_AT(int32_t, obj, 0x00) = 0;

    CSF_AT(int32_t, obj, 0x14) = srcB[0];
    CSF_AT(int32_t, obj, 0x18) = srcB[1];
    CSF_AT(int32_t, obj, 0x1C) = srcB[2];

    CSF_AT(float, obj, 0x04) = 1.0f;

    CSF_AT(int32_t, obj, 0x08) = srcA[0];
    CSF_AT(int32_t, obj, 0x0C) = srcA[1];
    const int32_t lastComponentA = srcA[2];
    CSF_AT(int32_t, obj, 0x10) = lastComponentA;

    return lastComponentA;
}

HOOK(0x004AD8E0, InitUnitScaleAndCopyTwoVec3, "auto");
