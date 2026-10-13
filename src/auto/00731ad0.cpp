#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

// Calls CanConfigureEffect for the given effect and, when it yields a non-null
// result, clears the byte at result+4 before returning that result.
static int __fastcall C_00867078_SetEffectState(C_00867078* self, void* /*edx*/, int effect)
{
    int result = CanConfigureEffect(self, nullptr, effect);
    if (result != 0)
    {
        CSF_AT(uint8_t, reinterpret_cast<void*>(result), 4) = 0;
    }
    return result;
}

HOOK(0x00731AD0, C_00867078_SetEffectState, "auto");
