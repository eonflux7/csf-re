#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound per the packet: C_00752db0_ClearFlag(void* self, void* edx).
CSF_FUNC(void, __fastcall, C_00752db0_ClearFlag, 0x00752DB0, (void* self, void* edx));

// Stdcall, 5 stack args (ret 0x14). Only the second argument is used: it is passed as `this` in ecx.
static int __stdcall NetGameDDL_ClearFlagOfArg2(int /*a1*/, int a2, int /*a3*/, int /*a4*/, int /*a5*/) {
    C_00752db0_ClearFlag(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2))), nullptr);
    return 0;
}

HOOK(0x0074CF80, NetGameDDL_ClearFlagOfArg2, "auto");
