#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Per-atomic visitor callback: receives an RpAtomic* and the caller's data pointer, returns an RpAtomic* (or null to stop).
using AtomicVisitorFn = void* (__cdecl*)(void* atomic, void* data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

static int __cdecl GfxFrames_ApplyToClumpAtomics(int clump, int data)
{
    AtomicVisitorFn visitor = reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x6C01A0));
    return RpClumpForAllAtomics(clump, visitor, data);
}

HOOK(0x6C01C0, GfxFrames_ApplyToClumpAtomics, "auto");
