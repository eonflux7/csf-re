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

// Visitor callback type for RpClumpForAllAtomics (called once per atomic).
typedef void* (__cdecl* AtomicVisitorFn)(void* atomic, void* data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

// Walks every atomic of the clump, calling the visitor (0x6C02F0) with the
// two-dword pair {pairA, pairB} as its data argument.
static int __cdecl GfxFrames_ForEachClumpAtomicWithPairB(int clump, int pairA, int pairB)
{
    int pairData[2];
    pairData[0] = pairA;
    pairData[1] = pairB;

    AtomicVisitorFn visitor = reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006C02F0));
    return RpClumpForAllAtomics(clump, visitor, static_cast<int>(reinterpret_cast<uintptr_t>(pairData)));
}

HOOK(0x006C0320, GfxFrames_ForEachClumpAtomicWithPairB, "auto");
