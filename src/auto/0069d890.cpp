#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(unsigned int, __cdecl, __controlfp, 0x007B070F,
         (unsigned int newValue, unsigned int mask));
CSF_GLOBAL(unsigned int, g_fpControlWord_8BB3F4, 0x008BB3F4);

static unsigned int __cdecl ErrorLog_SetFpuControlWordAndGetIt()
{
    // Save the incoming control state, then change it, then query the new state.
    g_fpControlWord_8BB3F4 = __controlfp(0, 0);
    __controlfp(0x10000u, 0x30300u);
    return __controlfp(0, 0);
}

HOOK(0x0069D890, ErrorLog_SetFpuControlWordAndGetIt, "auto");
