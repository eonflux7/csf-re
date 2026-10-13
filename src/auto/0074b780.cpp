#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetGhost DDL helper at 0x74B3C0. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2, so the
// helper sees a2 as its first stack argument and a3 as its second.
CSF_FUNC(char, __fastcall, NetGhostDdl_ProcessField, 0x0074B3C0, (void* self, void* edx, int a2, void* a3));

static char __stdcall NetGhostDdl_ForwardToProcessField(char* a1, char* a2, int a3)
{
    return NetGhostDdl_ProcessField(a1, nullptr,
                                    static_cast<int>(reinterpret_cast<uintptr_t>(a2)),
                                    reinterpret_cast<void*>(static_cast<uintptr_t>(a3)));
}

HOOK(0x0074B780, NetGhostDdl_ForwardToProcessField, "auto");
