// Commandos 3 / Strike Force (2006) — net_gathering_ddl / Commandos3DGameGatheringDDL.cpp
// 0x006A80D0: if the state field at +0x08 equals 2, store the signature 0x494C3830 at +0x2B0.
// __thiscall with 9 stack args (a2..a10), callee pops 36 bytes (ret 0x24).

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void __fastcall NetGatheringDdl_SetSignatureIfStateTwo(
    void* self, void* /*edx*/,
    int /*a2*/, int /*a3*/, int /*a4*/, int /*a5*/, int /*a6*/,
    int /*a7*/, int /*a8*/, int /*a9*/, int /*a10*/)
{
    if (CSF_AT(int32_t, self, 0x08) == 2)
        CSF_AT(int32_t, self, 0x2b0) = 0x494c3830;
}

HOOK(0x6A80D0, NetGatheringDdl_SetSignatureIfStateTwo, "auto");
