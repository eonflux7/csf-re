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

CSF_FUNC(void, __fastcall, sub_74BEE0, 0x0074BEE0, (void* self, void* edx, void* a2));

// Forwards its two stack arguments to sub_74BEE0. The disassembly loads ecx from
// [ebp+8] (a1) and pushes [ebp+0xc] (a2), so a1 is the callee's `this` and a2 its
// single stack argument. Callee pops 4 bytes, this wrapper pops 8.
static int __stdcall NetGhostDdl_ForwardToCalleeWithThis(char* a1, C_008622e8* a2)
{
    sub_74BEE0(reinterpret_cast<void*>(a1), nullptr, reinterpret_cast<void*>(a2));
    return 0;
}

HOOK(0x74C480, NetGhostDdl_ForwardToCalleeWithThis, "auto");
