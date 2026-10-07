#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0072CBD0 — trivial getter: returns the dword stored at this+0x154.
static int __fastcall CGuiSlider_GetField0154(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0154);
}

HOOK(0x0072CBD0, CGuiSlider_GetField0154, "auto");
