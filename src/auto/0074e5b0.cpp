#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to the NetGame DDL helper at 0x74E010. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 then a3 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetGameDdl_ProcessEntry, 0x0074E010, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetGameDdl_ForwardToProcessEntry(C_00858180* a1, C_00858180* a2, int a3)
{
    return NetGameDdl_ProcessEntry(a1, nullptr, a2, a3);
}

HOOK(0x0074E5B0, NetGameDdl_ForwardToProcessEntry, "auto");
