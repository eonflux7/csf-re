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

CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpClumpDestroy, 0x0058F740, (int a1));

// Original is __stdcall with ret 8 but takes `this` in ecx (mov edi, ecx).
// Written as __fastcall with two stack args so the callee pops 8 bytes.
static int __fastcall GfxTextureLoader_ReleaseIndexedResource(S_006d4450* self, void* /*edx*/, int a1, int a2)
{
    (void)self;
    (void)a1;

    // If the clump belongs to a world, remove it from that world first.
    if (S_006fdfe0_FindWorldFromClump(a2))
    {
        int worldOfClump = S_006fdfe0_FindWorldFromClump(a2);
        RpWorldRemoveClump(worldOfClump, a2);
    }
    return RpClumpDestroy(a2);
}

HOOK(0x6D4790, GfxTextureLoader_ReleaseIndexedResource, "auto");
