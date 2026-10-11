#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Table base pointer loaded from the game's global. Its value is used as a base address.
CSF_GLOBAL(uint32_t, g_ObjectPropertyTable, 0x008BB484);

// Returns the property word stored at (table base + index + 4), or 0 when index is 0.
static int __cdecl GfxObjectProps_GetObjectPropertyWord(int objectIndex) {
    if (objectIndex == 0) {
        return 0;
    }
    uintptr_t tableBase = static_cast<uintptr_t>(g_ObjectPropertyTable);
    void* entryAddress = reinterpret_cast<void*>(tableBase + static_cast<uintptr_t>(objectIndex));
    return CSF_AT(int, entryAddress, 4);
}

HOOK(0x006B8800, GfxObjectProps_GetObjectPropertyWord, "auto");
