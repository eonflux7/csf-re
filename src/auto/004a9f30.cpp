#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns non-zero when the point a1 (float[3]) lies strictly inside the sphere
// centred at this->xyz with radius-like value this->field_000C (compared squared).
static int __fastcall AiMoveTasks_IsPositionWithinRadius(void* self, void* /*edx*/, int a1)
{
    const float posX = CSF_AT(float, self, 0x0);
    const float posY = CSF_AT(float, self, 0x4);
    const float posZ = CSF_AT(float, self, 0x8);
    const float range = CSF_AT(float, self, 0xC);

    const float* target = reinterpret_cast<const float*>(a1);
    const float dx = target[0] - posX;
    const float dy = target[1] - posY;
    const float dz = target[2] - posZ;

    const float distSq = dx * dx + dy * dy + dz * dz;
    const float rangeSq = range * range;

    // Strictly inside: rangeSq > distSq (unordered/equal yields 0).
    if (rangeSq > distSq) {
        return 1;
    }
    return 0;
}

HOOK(0x004A9F30, AiMoveTasks_IsPositionWithinRadius, "auto");
