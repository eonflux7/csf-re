#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetGhost DDL helper at 0x74C260. The original takes (a1, a2, a3)
// as stdcall, passes a1 in ecx, and pushes a2 and a3 as the helper's two stack
// arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetGhostDdl_ProcessField, 0x0074C260, (void* self, void* edx, int a5, int a3));

static char __stdcall NetGhostDdl_ForwardToProcessField(void* a1, int a2, int a3)
{
    return NetGhostDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x0074C4A0, NetGhostDdl_ForwardToProcessField, "auto");
