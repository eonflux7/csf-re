#include <cstdint>
#include "hook.h"

// Zeroes the three dwords at *dst (a 3-component vector of 32-bit values) and returns dst.
static uint32_t* __stdcall SoundManager_ZeroVec3(uint32_t* dst)
{
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    return dst;
}

HOOK(0x005237A0, SoundManager_ZeroVec3, "auto");
