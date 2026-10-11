#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// RpWorldForAllWorldSectors(world, callback, userData): walks every world sector and calls the callback.
CSF_FUNC(int, __cdecl, RpWorldForAllWorldSectors, 0x58CFF0, (int a1, int (__cdecl* a2)(int*, int), int a3));

// Resolves the value stored at this+4 (a world pointer) by walking its sectors with the
// increment-counter callback; returns the counter accumulated in the stack slot.
static int __fastcall ResolveStreamSlotValue(void* self, void* /*edx*/)
{
    int worldPointer = CSF_AT(int, self, 0x4);
    int result = 0;
    int accumulatedValue = 0;
    if (worldPointer)
    {
        RpWorldForAllWorldSectors(
            worldPointer,
            reinterpret_cast<int (__cdecl*)(int*, int)>(CSF_ADDR_FN(0x6EE5A0)),
            static_cast<int>(reinterpret_cast<uintptr_t>(&accumulatedValue)));
        return accumulatedValue;
    }
    return result;
}

HOOK(0x6BC270, ResolveStreamSlotValue, "auto");
