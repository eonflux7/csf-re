#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetCogible DDL helper at 0x743280. The original takes
// (this, a2, a3) as stdcall and passes a1 in ecx, so no stack frame is
// needed beyond the arguments.
CSF_FUNC(char, __fastcall, sub_743280, 0x00743280, (void* self, void* edx, int a5, int a3));

static char __stdcall NetCogibleDdl_ForwardToHelper(void* a1, int a2, int a3)
{
    return sub_743280(a1, nullptr, a2, a3);
}

HOOK(0x00743430, NetCogibleDdl_ForwardToHelper, "auto");
