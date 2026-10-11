#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Base pointer of the entry table, loaded from the game's global. Its value is used as a base address.
CSF_GLOBAL(uint32_t, g_EntryTable_8BB480, 0x008BB480);

// Returns the word stored at (table base + index + 4), or 0 when index is 0.
static int __cdecl GfxObjectProps_GetEntryWord(int objectIndex) {
    if (objectIndex == 0) {
        return 0;
    }
    uintptr_t tableBase = static_cast<uintptr_t>(g_EntryTable_8BB480);
    void* entryAddress = reinterpret_cast<void*>(tableBase + static_cast<uintptr_t>(objectIndex));
    return CSF_AT(int, entryAddress, 4);
}

HOOK(0x006B87E0, GfxObjectProps_GetEntryWord, "auto");
