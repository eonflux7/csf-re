#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_74CE50, 0x0074CE50, (void* self, void* edx, void* a2));

// Forwards the fifth stack argument (a2) to the object passed as the fourth
// stack argument (arg4). The disassembly loads ecx from [ebp+0xc] (arg4), so
// arg4 is the callee's `this`, and pushes [ebp+0x14] (a2) as its single argument.
// The callee's return value in eax is returned unchanged.
// a1, a3 and a5 are never read. The original pops 20 bytes (ret 0x14).
static int __stdcall NetGameDDL_ForwardSecondArgToCallee(int a1, C_008622e8* arg4, int a3, C_008622e8* a2, int a5)
{
    (void)a1;
    (void)a3;
    (void)a5;
    return sub_74CE50(arg4, nullptr, a2);
}

HOOK(0x74CE30, NetGameDDL_ForwardSecondArgToCallee, "auto");
