#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0072D100 (16 bytes): stores a2 into CGuiSlider::m_bHorizontal (+0x150) and returns it.
static int __fastcall CGuiSlider_SetHorizontal(CGuiSlider* self, void* /*edx*/, int horizontal)
{
    self->m_bHorizontal = static_cast<uint32_t>(horizontal);
    return horizontal;
}

HOOK(0x72D100, CGuiSlider_SetHorizontal, "auto");
