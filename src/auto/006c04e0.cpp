#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(uint32_t, int);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

static int __cdecl GfxFrames_ProcessClumpAtomics(int clump)
{
    AtomicVisitorFn visitor =
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x6C01A0));
    int data = static_cast<int>(CSF_ADDR_FN(0x6C0230));
    return RpClumpForAllAtomics(clump, visitor, data);
}

HOOK(0x6C04E0, GfxFrames_ProcessClumpAtomics, "auto");
