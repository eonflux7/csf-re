#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetLayoutValue(CGuiControl* self, void* /*edx*/, int value)
{
    self->field_0124 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x4370C0, CGuiControl_SetLayoutValue, "auto");
