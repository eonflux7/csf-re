#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "core/memory.h"

// Forwards the constant arguments (13, 0) to virtual slot at vtbl + 0x134
// (slot index 0x134 / 4 = 77) of the receiver object.
static int __fastcall C_008334f0_DispatchStateWithArgs(C_008334f0* self, void* /*edx*/)
{
    typedef int (__fastcall* VtblSlotFn)(void* self, void* edx, int arg0, int arg1);
    VtblSlotFn slot = reinterpret_cast<VtblSlotFn>(self->vtbl[0x134 / 4]);
    return slot(self, nullptr, 13, 0);
}

HOOK(0x483630, C_008334f0_DispatchStateWithArgs, "auto");
