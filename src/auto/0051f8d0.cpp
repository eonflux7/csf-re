#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall FxParticleManager_FindEffectIndex(
    void* self, void* /*edx*/, int effectId)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x34);
    if (count == 0)
        return -1;

    const uint8_t* entry = CSF_AT(uint8_t*, self, 0x2C);
    for (uint32_t index = 0; index < count; ++index, entry += 0x14)
    {
        if (CSF_AT(int, entry, 0) == effectId)
            return static_cast<int>(index);
    }
    return -1;
}

HOOK(0x0051F8D0, FxParticleManager_FindEffectIndex, "auto");
