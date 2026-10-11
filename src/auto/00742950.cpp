#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: the callee takes (self, edx, a2, a3, a4, a5) with 4 stack args (ret 16).
CSF_FUNC(int, __fastcall, sub_751780, 0x00751780, (void* self, void* edx, void* a2, int a3, int a4, int a5));

// Original: stdcall, 7 stack args, ret 28. Forwards arg4 as `this`, a2 and a3..a5 as stack args.
static int __stdcall NetCogibleDDL_ForwardWithSelf(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return sub_751780(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x00742950, NetCogibleDDL_ForwardWithSelf, "auto");
