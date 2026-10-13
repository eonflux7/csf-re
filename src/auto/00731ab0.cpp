#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

static char __fastcall C_00867078_IsChannelActive(C_00867078* self, void* /*edx*/, int effect)
{
    int configuredEffect = CanConfigureEffect(self, nullptr, effect);
    if (configuredEffect != 0)
        return CSF_AT(char, reinterpret_cast<void*>(configuredEffect), 0x04);
    return 0;
}

HOOK(0x00731AB0, C_00867078_IsChannelActive, "auto");
