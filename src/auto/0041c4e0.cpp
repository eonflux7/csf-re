#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x418440, bound as in the packet.
CSF_FUNC(int, __fastcall, Vehicle_ResetWheelBindings, 0x00418440, (void* self, void* edx));

// Resets the wheel-related state of C_0082e3b8 (vehicle wheel object).
static int __fastcall C_0082e3b8_ResetWheelState(C_0082e3b8* self, void* /*edx*/)
{
    Vehicle_ResetWheelBindings(self, nullptr);
    self->field_00a4 = 0.0f;
    self->field_008c = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    return 0;
}

HOOK(0x0041C4E0, C_0082e3b8_ResetWheelState, "auto");
