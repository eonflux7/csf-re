#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to the NetCogible DDL helper at 0x743110. The original passes a1 in
// ecx and pushes a3 then a2, so the callee sees a2 as its first stack argument
// and a3 as its second. Callee return value (al) is passed through unchanged.
CSF_FUNC(char, __fastcall, sub_743110, 0x00743110, (void* self, void* edx, void* arg0, int a3));

static char __stdcall NetCogibleDdl_ForwardToHelper(C_00858180* a1, C_00858180* a2, int a3)
{
    return sub_743110(a1, nullptr, a2, a3);
}

HOOK(0x007433D0, NetCogibleDdl_ForwardToHelper, "auto");
