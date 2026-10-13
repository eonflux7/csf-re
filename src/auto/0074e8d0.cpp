#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetControlDDL helper at 0x74E700. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 and a3 as the
// helper's two stack arguments (a2 first, so it is the lower-address argument).
CSF_FUNC(char, __fastcall, NetControlDdl_ProcessField, 0x0074E700, (void* self, void* edx, int a2, void* a3));

static char __stdcall NetControlDdl_ForwardToProcessField(void* a1, int a2, void* a3)
{
    return NetControlDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x0074E8D0, NetControlDdl_ForwardToProcessField, "auto");
