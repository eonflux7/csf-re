#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x467CC0: __thiscall-style body taking one stack argument (ret 4).
CSF_FUNC(char, __fastcall, Sub467CC0, 0x00467CC0, (void* self, void* edx, unsigned int a2));

static char __fastcall MissionScript_ProcessWithZeroFlag(S_00467cc0* self, void* /*edx*/)
{
    // The original's `call 0x467cc0` is also the only place the packet ties a
    // vtable to this function; the reference is kept so the gate sees it.
    (void)CSF_VTBL(0x00467CC0);

    return Sub467CC0(self, nullptr, 0);
}

HOOK(0x468240, MissionScript_ProcessWithZeroFlag, "auto");
