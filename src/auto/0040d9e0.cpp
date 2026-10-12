#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Wheel_ResetBaseState, 0x0040D180, (void* self, void* edx));

static int __fastcall C_0082dcf0_ClearStateFields(C_0082dcf0* self, void* /*edx*/)
{
    Wheel_ResetBaseState(self, nullptr);
    self->field_0050 = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    return 0;
}

HOOK(0x0040D9E0, C_0082dcf0_ClearStateFields, "auto");
