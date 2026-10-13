#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(S_0041e5f0*, __cdecl, ParticleEmitters_GetSharedInstance, 0x006E2760, ());
CSF_FUNC(C_00863180*, __fastcall, C_00863180_Construct, 0x00702890, (C_00863180* self, void*));

// Stores the 8-byte pair and flag byte from the source record, then allocates
// and constructs the 32-byte helper object (or stores null if allocation fails).
static char __fastcall C_00861988_Initialize(void* self, void* /*edx*/, uint32_t* a2)
{
    CSF_AT(uint32_t, self, 0x08) = a2[0];
    CSF_AT(uint32_t, self, 0x0C) = a2[1];
    CSF_AT(uint8_t, self, 0x4C) = *(reinterpret_cast<uint8_t*>(a2) + 4);

    ParticleEmitters_GetSharedInstance();

    int rawMemory = CoreMemory_AllocUntagged(32);
    if (rawMemory != 0)
    {
        C_00863180* helper = reinterpret_cast<C_00863180*>(rawMemory);
        C_00863180* constructed = C_00863180_Construct(helper, nullptr);
        CSF_AT(uint32_t, self, 0x44) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(constructed));
    }
    else
    {
        CSF_AT(uint32_t, self, 0x44) = 0;
    }
    return 1;
}

HOOK(0x006DF810, C_00861988_Initialize, "auto");
