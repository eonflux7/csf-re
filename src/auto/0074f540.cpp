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

// Binding for the callee, copied exactly from the packet.
CSF_FUNC(void, __fastcall, sub_7534A0, 0x007534A0, (void* self, void* edx));

// Passes the second stack argument as `this` to sub_7534A0 (ecx = a2, edx unused).
// a1, a3, a4, a5 are unused; stdcall pops all five (ret 0x14).
static int __stdcall PlayerListDDL_ForwardSecondArgToLookup(int /*a1*/, int a2, int /*a3*/, int /*a4*/, int /*a5*/)
{
    sub_7534A0(reinterpret_cast<void*>(static_cast<intptr_t>(a2)), nullptr);
    return 0;
}

HOOK(0x0074F540, PlayerListDDL_ForwardSecondArgToLookup, "auto");
