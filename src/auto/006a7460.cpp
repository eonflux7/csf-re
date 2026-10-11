#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

static int __fastcall GatheringDdl_ResetPendingAndGetStatus(void* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    int result = CSF_AT(int, self, 0x08);
    CSF_AT(int, self, 0x338) = 0;
    if (!result)
        CSF_AT(int, self, 0x2B0) = 0x494C3830;
    return result;
}

HOOK(0x006A7460, GatheringDdl_ResetPendingAndGetStatus, "auto");
