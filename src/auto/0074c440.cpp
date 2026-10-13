#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetGhostDDL field helper at 0x74C050. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 and a3 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetGhostDdl_ProcessGhostField, 0x0074C050, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetGhostDdl_ForwardToProcessGhostField(void* a1, void* a2, int a3)
{
    return NetGhostDdl_ProcessGhostField(a1, nullptr, a2, a3);
}

HOOK(0x0074C440, NetGhostDdl_ForwardToProcessGhostField, "auto");
