#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPostman DDL helper at 0x7506A0. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 and a3 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetPostmanDdl_ProcessRecord, 0x007506A0, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetPostmanDdl_ForwardToProcessRecord(void* a1, void* a2, int a3)
{
    return NetPostmanDdl_ProcessRecord(a1, nullptr, a2, a3);
}

HOOK(0x007509E0, NetPostmanDdl_ForwardToProcessRecord, "auto");
