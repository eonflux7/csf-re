#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class reset routine (__thiscall, ret 0 -> pops nothing).
CSF_FUNC(int, __fastcall, sub_418440, 0x00418440, (void* self, void* edx));

// Runs the base reset, then clears the float at +0x8C to 0.0f and returns the base result.
static int __fastcall C_0082e280_ResetWheelState(C_0082e280* self, void* /*edx*/)
{
    int result = sub_418440(self, nullptr);
    self->field_008c = 0.0f;
    return result;
}

HOOK(0x0041B5B0, C_0082e280_ResetWheelState, "auto");
