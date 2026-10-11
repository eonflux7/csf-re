#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __stdcall FxParticleManager_ClearOutputState(int /*unused*/, int output)
{
    CSF_AT(uint32_t, reinterpret_cast<void*>(output), 0x8) = 0;
    return output;
}

HOOK(0x0051C290, FxParticleManager_ClearOutputState, "auto");
