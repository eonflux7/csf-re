#include <cstdint>

#include "globals.h"
#include "hook.h"

// Returns the address of particle slot `index` in the slot table whose base pointer
// is stored at this+0x64. Each slot is 0x3C (60) bytes.
static int __fastcall ParticleEmitters_LookupParticleSlot(void* self, void* /*edx*/, int index)
{
    int slotTableBase = CSF_AT(int, self, 0x64);
    return slotTableBase + index * 60;
}

HOOK(0x006E10F0, ParticleEmitters_LookupParticleSlot, "auto");
