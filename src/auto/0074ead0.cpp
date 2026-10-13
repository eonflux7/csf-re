#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: __thiscall callee (ecx = self) with 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, C_008622e8_WriteSpawnMessage_752BB0, 0x00752BB0, (C_008622e8* self, void* , C_008622e8* writer, int , int , int));

// Original: stdcall, 7 stack args, ret 28. Forwards arg4 as `this`, a2 as the writer,
// and a3..a5 as the remaining stack args, in the original's order.
static int __stdcall NetControlDDL_ForwardSpawnMessage(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return C_008622e8_WriteSpawnMessage_752BB0(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x0074EAD0, NetControlDDL_ForwardSpawnMessage, "auto");
