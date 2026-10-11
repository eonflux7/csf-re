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

// Forwards a graph-block parse request to the parser at 0x75E810.
// Original: __thiscall, 2 stack args (a2, a3), ret 8.
// ecx keeps `this` unchanged; the callee receives `this` as ecx, the raw a3 value
// in edx (left over from the original's `mov edx, [ebp+0xc]`), and a2 as its stack arg.
CSF_FUNC(char, __fastcall, WaypointGraph_ParseBlock, 0x0075E810, (void* self, void* edx, void* a2));

static char __fastcall C_0086c288_ParseBlockIfNonZero(void* self, void* /*edx*/, uint8_t* a2, int a3)
{
    char result = 0;
    if (a3)
    {
        // ecx = this (unchanged), edx = a3 value (left in edx by the original), stack arg = a2
        return WaypointGraph_ParseBlock(self, reinterpret_cast<void*>(static_cast<uintptr_t>(a3)), a2);
    }
    return result;
}

HOOK(0x0075ED00, C_0086c288_ParseBlockIfNonZero, "auto");
