#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: __fastcall callee, self = arg4, writer = a2, then three int stack args (ret 16).
CSF_FUNC(int, __fastcall, C_008622e8_WriteSpawnMessage_752D60, 0x00752D60, (C_008622e8* self, void* /*edx*/, C_008622e8* writer, int a3, int a4, int a5));

// Original: stdcall, 7 stack args, ret 28. Forwards arg4 as `this`, a2 as the writer, and a3..a5 as stack args.
static int __stdcall NetControlDDL_ForwardSpawnMessageWrite(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return C_008622e8_WriteSpawnMessage_752D60(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x0074E8F0, NetControlDDL_ForwardSpawnMessageWrite, "auto");
