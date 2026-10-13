#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: the callee takes (self, edx, a2, a3, a4, a5) with 4 stack args (ret 16).
CSF_FUNC(void*, __fastcall, sub_74B1C0, 0x0074B1C0, (void* self, void* edx, void* a2, int a3, int a4, int a5));

// Original: stdcall, 7 stack args, ret 28. Forwards arg2 as `this`, arg6 as the first stack arg,
// then arg4, arg5 and arg7. Returns the callee's result unchanged.
static void* __stdcall NetGhostDDL_ForwardWithSelf(int a1, int* a2, int a3, int a4, int a5, C_008622e8* a6, int a7)
{
    return sub_74B1C0(reinterpret_cast<void*>(a2), nullptr, reinterpret_cast<void*>(a6), a4, a5, a7);
}

HOOK(0x0074B7F0, NetGhostDDL_ForwardWithSelf, "auto");
