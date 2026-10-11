#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004E1300: __thiscall (ecx = this), bound per packet.
CSF_FUNC(int, __fastcall, C_0083e160_ResetBaseState, 0x004E1300, (void* self, void* edx));

// Clears the two counters at +0x18 and +0x1C of C_0083e6d8 after
// running the base-class reset. Returns 0.
static int __fastcall C_0083e6d8_ResetCounters(C_0083e6d8* self, void* /*edx*/)
{
    C_0083e160_ResetBaseState(self, nullptr);
    self->field_0018 = 0;
    self->field_001c = 0;
    return 0;
}

HOOK(0x004E2A80, C_0083e6d8_ResetCounters, "auto");
