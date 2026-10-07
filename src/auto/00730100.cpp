#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00730100: returns the dword stored at this+0x144 (field_0144).
static int __fastcall CGuiSlider_GetField0144(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0144);
}

HOOK(0x00730100, CGuiSlider_GetField0144, "auto");
