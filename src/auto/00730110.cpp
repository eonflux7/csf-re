#include "types/auto_classes.h"
#include "hook.h"

// Returns field at offset 0x148 (no stack arguments, plain getter).
static int __fastcall CGuiSlider_GetField0148(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0148);
}

HOOK(0x00730110, CGuiSlider_GetField0148, "auto");
