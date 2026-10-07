#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiControl::SetDraggable -- stores the byte flag at +0xCB (m_bDraggable) and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0xCB], al; ret 4  (only al is written, upper eax undefined).
static char __fastcall CGuiControl_SetDraggable(CGuiControl* self, void* /*edx*/, char value)
{
    self->m_bDraggable = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x7180B0, CGuiControl_SetDraggable, "auto");
