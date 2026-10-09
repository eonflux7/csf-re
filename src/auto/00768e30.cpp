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

// Forwards a dummy-mesh load request to the DUMMIES parser (sub_768CF0).
// Original: __thiscall, 2 stack args (a2, a3), ret 8.
// ecx keeps `this`; the callee receives `this` as ecx, the raw a3 value in edx
// (left over from the original's `mov edx, [ebp+0xc]`), and a2 as its stack arg.
CSF_FUNC(char, __fastcall, CDummyMesh_ParseDummiesBlock, 0x00768CF0, (void* self, void* edx, void* a2));

static char __fastcall CDummyMesh_LoadDummiesIfNonZero(void* self, void* /*edx*/, uint8_t* a2, int a3)
{
    char result = 0;
    if (a3)
    {
        // ecx = this (unchanged), edx = a3 value (left in edx by the original), stack arg = a2
        return CDummyMesh_ParseDummiesBlock(self, reinterpret_cast<void*>(static_cast<uintptr_t>(a3)), a2);
    }
    return result;
}

HOOK(0x00768E30, CDummyMesh_LoadDummiesIfNonZero, "auto");
