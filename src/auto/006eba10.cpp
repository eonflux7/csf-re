#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __stdcall GfxCollision_ClearIntersectionFlag(int /*a1*/, int a2)
{
    CSF_AT(uint8_t, reinterpret_cast<void*>(a2), 0x2) = 0;
    return 0;
}

HOOK(0x006EBA10, GfxCollision_ClearIntersectionFlag, "auto");
