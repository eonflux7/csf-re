#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);

// Zeroes the two consecutive dwords at g_EntryTable + entryOffset
// (the entry's first and second 32-bit words) and returns entryOffset.
static int __cdecl GfxObjectProps_ClearEntryTablePair(int entryOffset) {
    uint8_t* entryBase = static_cast<uint8_t*>(g_EntryTable);
    *reinterpret_cast<uint32_t*>(entryBase + entryOffset) = 0;
    *reinterpret_cast<uint32_t*>(entryBase + entryOffset + 4) = 0;
    return entryOffset;
}

HOOK(0x006B8580, GfxObjectProps_ClearEntryTablePair, "auto");
