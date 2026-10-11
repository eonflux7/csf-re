#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee: cdecl, two float args, result left in st0 (returned as double).
CSF_FUNC(double, __cdecl, sub_4068E0, 0x004068E0, (float a1, float a2));

// Original: __thiscall, no stack args (ret 0).
// Reads this[2] (+0x8) and this[3] (+0xC) and forwards them to the callee
// as (this[3], this[2]). The callee's result is returned unchanged.
static double __fastcall AiTaskStates_GetAngleDifference(float* self, void* /*edx*/)
{
    float angleA = self[3];
    float angleB = self[2];
    return sub_4068E0(angleA, angleB);
}

HOOK(0x0041E440, AiTaskStates_GetAngleDifference, "auto");
