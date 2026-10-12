#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the 1-byte flag at +0 to 1, stores a2 at +4, stores a3 at +8 and returns TRUE (al = 1).
static char __fastcall ParticleSystemDefs_SetSourceAndEnabledFlag(void* self, void* /*edx*/, int a2, char a3) {
    CSF_AT(int, self, 0x4) = a2;
    CSF_AT(uint8_t, self, 0x0) = 1;
    CSF_AT(uint8_t, self, 0x8) = static_cast<uint8_t>(a3);
    return 1;
}

HOOK(0x007012A0, ParticleSystemDefs_SetSourceAndEnabledFlag, "auto");
