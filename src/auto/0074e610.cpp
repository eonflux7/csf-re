#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetGame DDL entry handler at 0x74E440. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 then a3 as the
// handler's two stack arguments (a2 ends up first on the stack), so no frame
// is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetGameDdl_ProcessEntry, 0x0074E440, (void* self, void* edx, int a2, int a3));

static char __stdcall NetGameDdl_ForwardToProcessEntry(void* a1, int a2, int a3)
{
    return NetGameDdl_ProcessEntry(a1, nullptr, a2, a3);
}

HOOK(0x0074E610, NetGameDdl_ForwardToProcessEntry, "auto");
