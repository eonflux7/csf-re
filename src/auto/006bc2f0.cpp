#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Clump-walk primitive bound at its original address (rets 0).
// The visitor is passed as a raw address; no header typedef is assumed.
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, void* visitor, int data));

// Runs the atomic visitor at 0x6BC2D0 over every atomic of the clump and returns the clump unchanged.
static int __cdecl Environment_ApplyAtomicVisitorToClump(int clump)
{
    RpClumpForAllAtomics(clump, reinterpret_cast<void*>(CSF_ADDR_FN(0x006BC2D0)), 0);
    return clump;
}

HOOK(0x006BC2F0, Environment_ApplyAtomicVisitorToClump, "auto");
