#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPostman DDL helper at 0x750900. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2, so the
// helper receives a2 as its first stack argument and a3 as its second.
CSF_FUNC(char, __fastcall, NetPostmanDdl_ProcessField, 0x00750900, (void* self, void* edx, int a5, int a3));

static char __stdcall NetPostmanDdl_ForwardToProcessField(void* a1, int a2, int a3)
{
    return NetPostmanDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x00750A40, NetPostmanDdl_ForwardToProcessField, "auto");
