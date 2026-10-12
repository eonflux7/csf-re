#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Visitor signature used by RpClumpForAllAtomics: called per atomic with the caller's data.
typedef int (__cdecl* AtomicVisitorFn)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));

// Original calls RpClumpForAllAtomics(clump, PushValueToDynamicArray @ 0x6BD100, collector).
static int __cdecl SceneLoader_CollectSectorObjects(int clumpHandle, int collectorHandle)
{
    // Reset the collector's counter field at +0x08 before visiting atomics.
    CSF_AT(int, reinterpret_cast<void*>(collectorHandle), 0x8) = 0;

    return RpClumpForAllAtomics(
        clumpHandle,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006BD100)),
        collectorHandle);
}

HOOK(0x006C0B90, SceneLoader_CollectSectorObjects, "auto");
