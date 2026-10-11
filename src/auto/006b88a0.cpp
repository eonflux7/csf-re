#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global base pointer (dword_8BB480) used as the origin for the entry-table slot.
CSF_GLOBAL(uint8_t*, g_EntryTable, 0x8BB480);

// Stores `value` as a 32-bit integer at (base + offset) and returns `value`.
static int __cdecl GfxObjectProps_StoreEntryTableValue(int offset, int value)
{
    *reinterpret_cast<int*>(g_EntryTable + offset) = value;
    return value;
}

HOOK(0x006B88A0, GfxObjectProps_StoreEntryTableValue, "auto");
