#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072FF90: returns the dword at offset 0x13c of the object.
static int __fastcall CGuiSlider_GetField013c(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<int>(self->field_013c);
}

HOOK(0x72FF90, CGuiSlider_GetField013c, "auto");
