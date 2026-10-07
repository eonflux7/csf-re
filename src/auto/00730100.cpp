#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00730100: returns the dword stored at this+0x144 (m_pArrowUp).
static int __fastcall CGuiSlider_GetField0144(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<int>(self->m_pArrowUp);
}

HOOK(0x00730100, CGuiSlider_GetField0144, "auto");
