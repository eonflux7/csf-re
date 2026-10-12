#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns the address of entry `index` in the table whose base pointer is stored at
// this+0x74. Each entry is 0x3C (60) bytes.
static int __fastcall ParticleEmitters_LookupSlotAddress(void* self, void* /*edx*/, int index)
{
    int tableBase = CSF_AT(int, self, 0x74);
    return tableBase + index * 0x3C;
}

HOOK(0x006E1110, ParticleEmitters_LookupSlotAddress, "auto");
