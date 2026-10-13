#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetProyectil DDL helper at 0x747DC0. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2, so the
// helper receives a2 as its first stack argument and a3 as its second.
CSF_FUNC(char, __fastcall, NetProyectilDdl_ProcessField, 0x00747DC0, (void* self, void* edx, int a5, int a3));

static char __stdcall NetProyectilDdl_ForwardToProcessField(void* a1, int a2, int a3)
{
    return NetProyectilDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x00747E90, NetProyectilDdl_ForwardToProcessField, "auto");
