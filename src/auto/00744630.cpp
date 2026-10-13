#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_64D6B0, 0x0064D6B0, (int a1, uint32_t a2));
CSF_GLOBAL(std::uint32_t, g_NetPlayerClassId, 0x008BC15C);

static int __cdecl NetPlayerDdl_CallWithContext(int argument)
{
    return sub_64D6B0(static_cast<int>(g_NetPlayerClassId), static_cast<uint32_t>(argument));
}

HOOK(0x00744630, NetPlayerDdl_CallWithContext, "auto");
