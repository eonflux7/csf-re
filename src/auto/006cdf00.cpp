#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// 0x006CDF00
// Returns the signed number of 100-byte elements held between the
// array begin pointer at +0x08 and the array end pointer at +0x0C,
// or 0 when the begin pointer is null. The original computes this with
// the same magic-number signed division by 100.
static int __fastcall CollisionShapes_GetElementCount(void* self, void* /*edx*/)
{
    const int32_t begin = CSF_AT(int32_t, self, 0x08);
    if (begin == 0)
        return 0;

    const int32_t end = CSF_AT(int32_t, self, 0x0C);
    return (end - begin) / 100;
}

HOOK(0x006CDF00, CollisionShapes_GetElementCount, "auto");
