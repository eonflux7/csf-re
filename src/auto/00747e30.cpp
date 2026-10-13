#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetProyectil DDL helper at 0x747D40. The original takes
// (a1, a2, a3) as stdcall, passes a1 in ecx, and pushes a3 then a2 as the
// helper's two stack arguments, so the wrapper needs no frame of its own.
CSF_FUNC(char, __fastcall, NetProyectilDdl_ProcessField, 0x00747D40, (void* self, void* edx, void* arg0, int* a3));

static char __stdcall NetProyectilDdl_ForwardToProcessField(void* a1, void* a2, int* a3)
{
    return NetProyectilDdl_ProcessField(a1, nullptr, a2, a3);
}

HOOK(0x00747E30, NetProyectilDdl_ForwardToProcessField, "auto");
