#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the first dword pointed to by the object's +0x34 pointer, or 0 when
// the +0x3C count is zero. __thiscall with no stack arguments, so ret 0.
static int __fastcall GfxWater_GetLightingSource(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x3C) != 0) {
        uint32_t* lightingSourceSlot = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0x34));
        return static_cast<int>(*lightingSourceSlot);
    }
    return 0;
}

HOOK(0x6D13C0, GfxWater_GetLightingSource, "auto");
