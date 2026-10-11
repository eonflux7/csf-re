#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the int stored at (base + a1), where base is the dword at 0x8BB484.
// The original reloads the global on every call and adds the signed argument
// to it as a plain integer offset, then loads the dword at that address.
CSF_GLOBAL(uint32_t, g_PropertyTableBase, 0x8BB484);

static int __cdecl GfxProps_ReadPropertyTableEntry(int a1) {
    uint32_t address = g_PropertyTableBase + static_cast<uint32_t>(a1);
    return *reinterpret_cast<int*>(address);
}

HOOK(0x006B88C0, GfxProps_ReadPropertyTableEntry, "auto");
