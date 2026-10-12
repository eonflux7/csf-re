#include <cstdint>
#include "hook.h"

// Writes (self - a3) into a2 as a 3-component float vector and returns a2.
// Original: __thiscall, 2 stack args, ret 8.
static float* __fastcall BuildFacingVectorBetween(float* self, void* /*edx*/, float* a2, float* a3) {
    float x = self[0] - a3[0];
    float y = self[1] - a3[1];
    float z = self[2] - a3[2];

    a2[0] = x;
    a2[1] = y;
    a2[2] = z;
    return a2;
}

HOOK(0x402030, BuildFacingVectorBetween, "auto");
