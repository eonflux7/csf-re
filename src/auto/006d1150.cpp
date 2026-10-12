#include <cstdint>
#include "hook.h"

// Component-wise sum of two 4-float vectors: dest = self + src. Returns dest.
// Original: __thiscall (ecx = self) with two stack args, ret 8.
static float* __fastcall GfxLights_AddVector4ToDest(float* self, void* /*edx*/, float* dest, float* src) {
    dest[0] = src[0] + self[0];
    dest[1] = src[1] + self[1];
    dest[2] = src[2] + self[2];
    dest[3] = src[3] + self[3];
    return dest;
}

HOOK(0x6D1150, GfxLights_AddVector4ToDest, "auto");
