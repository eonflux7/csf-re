#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_64D6B0, 0x0064D6B0, (int a1, uint32_t a2));
CSF_GLOBAL(int, g_NetCogibleDdlContext, 0x008BC128);

static int __cdecl NetCogibleDdl_CallWithContext(int a1)
{
    return sub_64D6B0(g_NetCogibleDdlContext, static_cast<uint32_t>(a1));
}

HOOK(0x742060, NetCogibleDdl_CallWithContext, "auto");
