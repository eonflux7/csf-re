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

// Looks up the RpWorld that owns an RpClump.
CSF_FUNC(int, __cdecl, S_006fdfe0_FindWorldFromClump, 0x0058DF40, (int a1));
// Removes an RpClump from an RpWorld.
CSF_FUNC(int, __cdecl, RpWorldRemoveClump, 0x0058E310, (int a1, int a2));

// Removes a PTank clump from its owning world and returns the clump unchanged.
static int __cdecl PTank_RemoveClumpFromWorld(int clump)
{
    int world = S_006fdfe0_FindWorldFromClump(clump);
    RpWorldRemoveClump(world, clump);
    return clump;
}

HOOK(0x006EE5B0, PTank_RemoveClumpFromWorld, "auto");
