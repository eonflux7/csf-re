#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static bool __stdcall FxParticleManager_IsType4Unflagged(void* particle)
{
    if (CSF_AT(std::uint32_t, particle, 0x04) == 4)
        return CSF_AT(std::uint8_t, particle, 0x71) == 0;
    return false;
}

HOOK(0x0051CFC0, FxParticleManager_IsType4Unflagged, "auto");
