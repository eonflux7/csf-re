#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __stdcall FxParticleManager_IsType40Or41(void* object)
{
    const int type = CSF_AT(int, object, 0x4);
    return type == 40 || type == 41 ? 1 : 0;
}

HOOK(0x0051DF40, FxParticleManager_IsType40Or41, "auto");
