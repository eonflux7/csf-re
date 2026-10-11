#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Lee el valor de g_EntryTable[entryIndex + 4] (puntero a entrada). Si la entrada
// existe y su byte en +0x11 es distinto de cero, marca *outFlag = 1.
// Devuelve entryIndex sin modificar.
CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);

static int __cdecl GfxObjectProps_SetOutFlagIfEntryFlagSet(int entryIndex, uint8_t* outFlag) {
    int result = entryIndex;
    if (entryIndex != 0) {
        uint32_t entry = CSF_AT(uint32_t, g_EntryTable, entryIndex + 4);
        if (entry != 0) {
            uint8_t entryFlag = CSF_AT(uint8_t, reinterpret_cast<void*>(entry), 0x11);
            if (entryFlag != 0) {
                *outFlag = 1;
            }
        }
    }
    return result;
}

HOOK(0x006B8FC0, GfxObjectProps_SetOutFlagIfEntryFlagSet, "auto");
