#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: the callee takes (self, edx, output, a3, a4, a5) with 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, C_008622e8_WriteSpawnMessage_7514D0, 0x007514D0, (C_008622e8* self, void* , C_008622e8* output, int , int , int));

// Original: stdcall, 7 stack args, ret 28. Forwards arg4 as `this`, a2 as the output buffer,
// and a3..a5 as the remaining stack args. Parameter _10 is not used by the original.
static int __stdcall NetProyectilDDL_ForwardSpawnMessage(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return C_008622e8_WriteSpawnMessage_7514D0(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x007479F0, NetProyectilDDL_ForwardSpawnMessage, "auto");
