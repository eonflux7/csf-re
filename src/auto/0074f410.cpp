#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: the callee takes (self, edx, writer, a3, a4, a5) with 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, C_008622e8_SerializeSpawnMessage_7537E0, 0x007537E0, (C_008622e8* self, void* /*edx*/, C_008622e8* writer, int, int, int));

// Original: stdcall, 7 stack args, ret 28. Forwards arg4 as `this`, a2 as the writer, and a3..a5 as stack args.
static int __stdcall NetPlayerListDDL_ForwardSpawnMessageSerialize(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return C_008622e8_SerializeSpawnMessage_7537E0(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x0074F410, NetPlayerListDDL_ForwardSpawnMessageSerialize, "auto");
