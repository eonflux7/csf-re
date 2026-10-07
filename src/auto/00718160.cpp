#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00718160: returns the 32-bit field at offset 0x00A4 of CGuiControl.
static int __fastcall CGuiControl_GetParent(CGuiControl* self, void* /*edx*/)
{
    return static_cast<int>(self->m_pParent);
}

HOOK(0x00718160, CGuiControl_GetParent, "auto");
