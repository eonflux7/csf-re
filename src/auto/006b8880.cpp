#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the int stored at (table + a1), where table is the pointer stored at 0x8BB480.
// The original reloads the global on every call and adds the signed argument
// to the table pointer as a plain integer offset.
CSF_GLOBAL(uint32_t, g_EntryTable_8BB480, 0x8BB480);

static int __cdecl GfxObjectProps_ReadEntryTableValue(int a1) {
    uint32_t address = g_EntryTable_8BB480 + static_cast<uint32_t>(a1);
    return *reinterpret_cast<int*>(address);
}

HOOK(0x006B8880, GfxObjectProps_ReadEntryTableValue, "auto");
