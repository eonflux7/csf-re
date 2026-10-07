#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006C2AC0
// Reads the 32-bit value stored at +0x4 of the passed object and returns it
// shifted left by 4 (i.e. multiplied by 16, 32-bit truncated). Most likely an
// element-count -> byte-size conversion for fixed 16-byte elements.
static int __cdecl GetScaledCountBy16(void* object)
{
    return static_cast<int>(CSF_AT(uint32_t, object, 0x4) << 4);
}

HOOK(0x6C2AC0, GetScaledCountBy16, "auto");
