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

CSF_FUNC(int, __fastcall, NetPostmanDdl_HandleSecondArg, 0x0074FCE0, (void* self, void* edx, void* a2));

// Forwards the fourth stack argument (a2) to the object passed as the second
// stack argument (arg4). The disassembly loads ecx from [ebp+0xc] (arg4), so
// arg4 is the callee's `this`, and pushes [ebp+0x14] (a2) as its single argument.
// a1, a3 and a5 are never read. The callee's result is returned unchanged.
static int __stdcall NetPostmanDdl_ForwardToSecondArg(int a1, C_008622e8* arg4, int a3, C_008622e8* a2, int a5)
{
    (void)a1;
    (void)a3;
    (void)a5;
    return NetPostmanDdl_HandleSecondArg(arg4, nullptr, a2);
}

HOOK(0x74FCC0, NetPostmanDdl_ForwardToSecondArg, "auto");
