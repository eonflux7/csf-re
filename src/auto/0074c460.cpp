#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Helper at 0x74BD70 (NetGhostDDL). Takes a1 in ecx and two stack arguments.
// The original pushes a2 first and a3 last, so a3 is the callee's first stack
// parameter and a2 is the second. Bound with the packet's signature.
CSF_FUNC(void, __fastcall, NetGhostDdl_ProcessFieldHelper, 0x0074BD70, (void* self, void* edx, void* a2, int a3));

// Forwards to the helper at 0x74BD70. The original keeps a1 in ecx and passes
// (a3, a2) as the helper's two stack arguments, so no frame is needed. The
// stdcall pop of 12 bytes matches the original's ret 0xc.
static void __stdcall NetGhostDdl_ForwardToProcessField(char* a1, int a2, void* a3)
{
    NetGhostDdl_ProcessFieldHelper(a1, nullptr, a3, a2);
}

HOOK(0x0074C460, NetGhostDdl_ForwardToProcessField, "auto");
