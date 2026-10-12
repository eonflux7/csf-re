#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static char __stdcall NetGameDdl_GetSecondArgFlag(
    int /*a1*/, int object, int /*a3*/, int /*a4*/)
{
    return CSF_AT(char, reinterpret_cast<void*>(object), 0x19E);
}

HOOK(0x0074CF70, NetGameDdl_GetSecondArgFlag, "auto");
