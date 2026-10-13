#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPostman DDL helper at 0x74F7C0. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetPostmanDdl_ProcessField, 0x0074F7C0, (void* self, void* edx, int a2, void* a3));

static char __stdcall NetPostmanDdl_ForwardToProcessField(void* a1, int a2, void* a3)
{
    return NetPostmanDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x0074FC80, NetPostmanDdl_ForwardToProcessField, "auto");
