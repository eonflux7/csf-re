#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RpWorldForAllClumps, 0x0058CEF0, (int a1, int (__cdecl* a2)(uint32_t*, int), int a3));

// Original: resets the collector's counter at +0x08, then walks every clump in the world
// with PushValueToDynamicArray (0x6BD100) as the visitor, passing the collector along.
static int __cdecl GfxEnvironment_CollectClumps(int worldHandle, int collectorHandle)
{
    CSF_AT(int, reinterpret_cast<void*>(collectorHandle), 0x8) = 0;

    return RpWorldForAllClumps(
        worldHandle,
        reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006BD100)),
        collectorHandle);
}

HOOK(0x006C0BC0, GfxEnvironment_CollectClumps, "auto");
