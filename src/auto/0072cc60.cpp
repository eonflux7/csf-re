#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072CC60 (7 bytes) — returns CGuiSlider::field_014c (dword at this+0x14c).
static int __fastcall CGuiSlider_GetField014c(CGuiSlider* self, void* /*edx*/)
{
    return (int)self->field_014c;
}

HOOK(0x0072CC60, CGuiSlider_GetField014c, "auto");
