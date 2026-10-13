#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, sub_64D6B0, 0x0064D6B0, (int a1, uint32_t a2));
CSF_GLOBAL(uint32_t, g_NetProyectilClassId, 0x008BC160);

static int __cdecl NetProyectilDdl_CreateRecordBlock(int a1)
{
    return sub_64D6B0(static_cast<int>(g_NetProyectilClassId), static_cast<uint32_t>(a1));
}

HOOK(0x7474E0, NetProyectilDdl_CreateRecordBlock, "auto");
