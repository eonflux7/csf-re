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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(char, __fastcall, ParticleEmitters_ReleaseAll, 0x006E08D0, (void* self, void* edx));

// Destructor body for C_00860d90 (vtable 0x860D90). a2 bit 0 set = also free the object itself.
static void** __fastcall C_00860d90_Destroy(C_00860d90* self, void* /*edx*/, char a2)
{
    // The emitter-list member lives at offset +4 inside the object.
    void* emitterListSlot = reinterpret_cast<uint8_t*>(self) + 4;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860D90);

    ParticleEmitters_ReleaseAll(emitterListSlot, nullptr);
    ParticleEmitters_ReleaseAll(emitterListSlot, nullptr);

    uint32_t& heapBlock = CSF_AT(uint32_t, self, 4);
    if (heapBlock != 0)
    {
        Mem_Free(reinterpret_cast<void*>(heapBlock));
        CSF_AT(uint32_t, self, 4) = 0;
    }

    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<void**>(self);
}

HOOK(0x006D00B0, C_00860d90_Destroy, "auto");
