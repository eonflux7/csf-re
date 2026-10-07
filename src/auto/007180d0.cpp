#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiControl::SetMouseEnabled -- stores the byte at +0xCA (m_bMouseEnabled) and returns it in al.
// Original: mov al, [ebp+8]; mov [ecx+0xCA], al; pop ebp; ret 4
static char __fastcall CGuiControl_SetMouseEnabled(CGuiControl* self, void* /*edx*/, char enabled)
{
    self->m_bMouseEnabled = static_cast<uint8_t>(enabled);
    return enabled;
}

HOOK(0x7180D0, CGuiControl_SetMouseEnabled, "auto");
