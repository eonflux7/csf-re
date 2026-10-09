#include <cstdint>
#include "globals.h"
#include "hook.h"

typedef int(__cdecl* AtomicVisitorFn)(uint32_t*, int);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x58EF00, (int clump, AtomicVisitorFn visitor, int data));

static int __cdecl SceneLoader_ApplyAtomicCallbackToClump(int clump, int data)
{
    RpClumpForAllAtomics(clump, reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x4FEF20)), data);
    return clump;
}

HOOK(0x4FF000, SceneLoader_ApplyAtomicCallbackToClump, "auto");
