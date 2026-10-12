#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the emitter's active flag at +0, stores the 8-bit argument at +8,
// and initializes the float at +4 to 0.2f (bit pattern 0x3E4CCCCD). Returns true.
static bool __fastcall ParticleSystemDefs_InitSpawnState(void* self, void* /*edx*/, char a2)
{
    CSF_AT(uint8_t, self, 0x08) = static_cast<uint8_t>(a2);
    CSF_AT(uint8_t, self, 0x00) = 1;
    CSF_AT(float, self, 0x04) = 0.2f;
    return true;
}

HOOK(0x007012C0, ParticleSystemDefs_InitSpawnState, "auto");
