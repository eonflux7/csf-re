#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Bound by address: __thiscall callee, (self, edx, destination, 3 int stack args), ret 16.
CSF_FUNC(int, __fastcall, C_008622e8_WriteSpawnFields, 0x00753280, (C_008622e8* self, void* /*edx*/, C_008622e8* destination, int a3, int a4, int a5));

// Original: stdcall, 7 stack args, ret 28. arg4 is passed as `this`, a2 as the destination,
// and a3..a5 are forwarded in order. a1 and _10 are not used by the original.
static int __stdcall NetGhostDDL_WriteSpawnFieldsForward(int a1, C_008622e8* arg4, int unused10, int a3, int a4, C_008622e8* a2, int a5)
{
    return C_008622e8_WriteSpawnFields(arg4, nullptr, a2, a3, a4, a5);
}

HOOK(0x0074B7A0, NetGhostDDL_WriteSpawnFieldsForward, "auto");
