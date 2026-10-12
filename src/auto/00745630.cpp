#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __stdcall NetPlayerDdl_GetPlayerFlag(
    int /*a1*/, int player, int /*a3*/, int /*a4*/)
{
    return CSF_AT(char, reinterpret_cast<void*>(player), 0x20C);
}

HOOK(0x00745630, NetPlayerDdl_GetPlayerFlag, "auto");
