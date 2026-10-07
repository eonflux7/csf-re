#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072CC60 (7 bytes) — returns CGuiSlider::m_pThumb (dword at this+0x14c).
static int __fastcall CGuiSlider_GetField014c(CGuiSlider* self, void* /*edx*/)
{
    return (int)self->m_pThumb;
}

HOOK(0x0072CC60, CGuiSlider_GetField014c, "auto");
