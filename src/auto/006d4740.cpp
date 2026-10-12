#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpClumpClone, 0x0058FC60, (int a1));

// Each entry is 60 bytes, located at (table base + 60 * index).
// Entry layout used here: +0x1C remaining-count, +0x20 pointer to clump array, +0x28 array length.
static int __fastcall GfxObject_AcquireIndexedResource(S_006d4450* self, void* /*edx*/, int a2)
{
    uint32_t tableBase = self->field_0008;
    uint32_t entryAddress = tableBase + 60u * static_cast<uint32_t>(a2);
    uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);

    int remainingCount = CSF_AT(int, entry, 0x1C);

    if (remainingCount == -1)
    {
        // Unlimited entry: clone the first clump of the array.
        uint32_t clumpArray = CSF_AT(uint32_t, entry, 0x20);
        int firstClump = CSF_AT(int, reinterpret_cast<void*>(clumpArray), 0);
        return RpClumpClone(firstClump);
    }

    if (remainingCount <= 0)
    {
        return 0;
    }

    uint32_t clumpArray = CSF_AT(uint32_t, entry, 0x20);
    int arrayLength = CSF_AT(int, entry, 0x28);
    int result = CSF_AT(int, reinterpret_cast<void*>(clumpArray), 4 * arrayLength - 4);

    CSF_AT(int, entry, 0x1C) = remainingCount - 1;
    CSF_AT(int, entry, 0x28) = CSF_AT(int, entry, 0x28) - 1;
    return result;
}

HOOK(0x6D4740, GfxObject_AcquireIndexedResource, "auto");
