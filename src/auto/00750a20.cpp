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

CSF_FUNC(void, __fastcall, sub_750500, 0x00750500, (void* self, void* edx, void* a2));

// Forwards the second stack argument (a2) to the first stack argument (a1),
// which the original loads into ecx as the `this` of the callee. a1 is never
// read as data. The callee is bound as void per the packet, so the value left
// in eax by the call is not carried through.
static int __stdcall NetPostmanDdl_ForwardToHandler(void* a1, C_008622e8* a2)
{
    sub_750500(a1, nullptr, a2);
    return 0;
}

HOOK(0x750A20, NetPostmanDdl_ForwardToHandler, "auto");
