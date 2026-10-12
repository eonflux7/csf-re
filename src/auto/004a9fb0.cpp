#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

// __thiscall(this, float* a2, float* a3), ret 8 -> __fastcall(self, edx, a2, a3).
// Stores the difference vector (a3 - a2) into this (offsets 0x00..0x08), copies the raw
// bits of a2 into 0x0C..0x14, then normalizes the stored vector and returns its result.
static float* __fastcall C_00833b88_SetProbeSegment(C_00833b88* self, void* /*edx*/, float* a2, float* a3)
{
    float diffX = a3[0] - a2[0];
    float diffY = a3[1] - a2[1];
    float diffZ = a3[2] - a2[2];

    CSF_AT(float, self, 0x0) = diffX;
    self->field_0004 = *reinterpret_cast<uint32_t*>(&diffY);
    CSF_AT(float, self, 0x8) = diffZ;

    const uint32_t* a2Bits = reinterpret_cast<const uint32_t*>(a2);
    self->field_000c = a2Bits[0];
    CSF_AT(uint32_t, self, 0x10) = a2Bits[1];
    CSF_AT(uint32_t, self, 0x14) = a2Bits[2];

    return NormalizeVector3(self, nullptr);
}

HOOK(0x004A9FB0, C_00833b88_SetProbeSegment, "auto");
