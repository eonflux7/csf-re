#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x004D1700: __thiscall (ecx = this), bound per packet.
CSF_FUNC(int, __fastcall, TCharTask_ResetBaseState, 0x004D1700, (void* self, void* edx));

// Runs the base-class reset, then clears the two counters at +0x40 and +0x44
// of C_0083b2e0. Returns 0.
static int __fastcall C_0083b2e0_ResetCounters(C_0083b2e0* self, void* /*edx*/)
{
    TCharTask_ResetBaseState(self, nullptr);
    self->field_0040 = 0;
    self->field_0044 = 0;
    return 0;
}

HOOK(0x004CF550, C_0083b2e0_ResetCounters, "auto");
