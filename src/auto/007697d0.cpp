#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns the address of entry `index` in the table whose base pointer is stored at
// this+0x08. Each entry is 0x44 (68) bytes.
static int __fastcall MissionScript_LookupMaskedEntry(void* self, void* /*edx*/, int index)
{
    int tableBase = CSF_AT(int, self, 0x08);
    return tableBase + index * 0x44;
}

HOOK(0x007697D0, MissionScript_LookupMaskedEntry, "auto");
