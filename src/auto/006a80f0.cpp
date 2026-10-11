// Commandos: Strike Force (2006) - net_gathering_ddl / Commandos3DGameGatheringDDL.cpp
// 0x006A80F0: if the state field at +0x08 equals 7, store the 0x494C3830 marker at +0x2B0.

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// __thiscall with six stack arguments (ret 0x18): ecx = this, edx unused, callee pops 24 bytes.
static void __fastcall NetGatheringDDL_SetMarkerIfStateSeven(void* self, void* /*edx*/,
                                                             int /*a2*/, int /*a3*/, int /*a4*/,
                                                             int /*a5*/, int /*a6*/, int /*a7*/)
{
    if (CSF_AT(int32_t, self, 0x8) == 7)
        CSF_AT(int32_t, self, 0x2B0) = 0x494C3830;
}

HOOK(0x6A80F0, NetGatheringDDL_SetMarkerIfStateSeven, "auto");
