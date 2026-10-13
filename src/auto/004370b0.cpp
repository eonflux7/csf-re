#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetField0120(CGuiControl* self, void* /*edx*/, int value)
{
    self->field_0120 = value;
    return value;
}

HOOK(0x4370B0, CGuiControl_SetField0120, "auto");
