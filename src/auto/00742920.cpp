#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetCogible DDL helper at 0x742080. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 and a3 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetCogibleDdl_ProcessField, 0x00742080, (void* self, void* edx, int a2, void* a3));

static char __stdcall NetCogibleDdl_ForwardToProcessField(void* a1, int a2, void* a3)
{
    return NetCogibleDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x00742920, NetCogibleDdl_ForwardToProcessField, "auto");
