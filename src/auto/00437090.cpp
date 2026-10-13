#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CGuiControl_SetStoredValue0118(CGuiControl* self, void* /*edx*/, int value)
{
    self->field_0118 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x437090, CGuiControl_SetStoredValue0118, "auto");
