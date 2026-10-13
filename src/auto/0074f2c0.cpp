#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPlayerList DDL helper at 0x74F170. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2 as the
// helper's two stack arguments, so no frame is needed beyond the arguments.
CSF_FUNC(char, __fastcall, NetPlayerListDdl_ProcessField, 0x0074F170, (void* self, void* edx, int a5, int a3));

static char __stdcall NetPlayerListDdl_ForwardToProcessField(int* a1, int a2, int a3)
{
    return NetPlayerListDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x0074F2C0, NetPlayerListDdl_ForwardToProcessField, "auto");
