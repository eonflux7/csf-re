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

CSF_FUNC(void, __fastcall, sub_74EF80, 0x0074EF80, (void* self, void* edx, void* a2));

// Forwards the second stack argument (a2) to the callee with the first stack
// argument (a1) as `this`. The disassembly loads ecx from [ebp+8] (a1) and
// pushes [ebp+0xc] (a2) as the callee's single argument. The callee is bound
// as void per the packet, so the eax passthrough of the original cannot be
// read here; the return value is a known gap (see metadata notes).
static int __stdcall NetPlayerListDdl_ForwardToSecondArg(void* a1, C_008622e8* a2)
{
    sub_74EF80(a1, nullptr, a2);
    return 0;
}

HOOK(0x74F2A0, NetPlayerListDdl_ForwardToSecondArg, "auto");
