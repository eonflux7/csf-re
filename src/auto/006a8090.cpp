#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets the 0x494C3830 marker at +0x2B0 when the state at +0x08 equals 8.
static void __fastcall NetGatheringDdl_SetMarkerIfStateEight(void* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x08) == 8)
        CSF_AT(uint32_t, self, 0x2B0) = 0x494C3830u;
}

HOOK(0x006A8090, NetGatheringDdl_SetMarkerIfStateEight, "auto");
