#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetProyectil DDL helper at 0x74F6E0. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a2 then a3, so the
// helper receives a3 as its first stack argument and a2 as its second.
CSF_FUNC(void, __fastcall, NetProyectilDdl_ProcessField, 0x0074F6E0, (void* self, void* edx, void* a2, int a3));

static int __stdcall NetProyectilDdl_ForwardToProcessField(void* a1, int a2, void* a3)
{
    NetProyectilDdl_ProcessField(a1, nullptr, a3, a2);
    return 0;
}

HOOK(0x00747E50, NetProyectilDdl_ForwardToProcessField, "auto");
