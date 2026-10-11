#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Wheel_ResetBaseState, 0x00418440, (void* self, void* edx));

static int __fastcall C_0082e428_ClearWheelFields(C_0082e428* self, void* /*edx*/)
{
    Wheel_ResetBaseState(self, nullptr);
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_00a0 = 0.0f;
    return 0;
}

HOOK(0x0041CFF0, C_0082e428_ClearWheelFields, "auto");
