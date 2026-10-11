#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Base pointer of the entry table. The original loads this global's value and uses it as a base address.
CSF_GLOBAL(uint32_t, g_EntryTable, 0x008BB47C);

// Returns the sector data word stored at (entry table base + index + 4), or 0 when index is 0.
static int __cdecl GfxObjectProps_GetObjectSectorData(int objectIndex) {
    if (objectIndex == 0) {
        return objectIndex;
    }
    uintptr_t tableBase = static_cast<uintptr_t>(g_EntryTable);
    void* entryAddress = reinterpret_cast<void*>(tableBase + static_cast<uintptr_t>(objectIndex));
    return CSF_AT(int, entryAddress, 4);
}

HOOK(0x006B87C0, GfxObjectProps_GetObjectSectorData, "auto");
