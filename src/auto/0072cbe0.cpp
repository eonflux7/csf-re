#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiSlider::SetConsole -- stores the byte flag at +0x169 (m_bConsole) and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x169], al; ret 4  (returns the value in al only).
static char __fastcall CGuiSlider_SetConsole(CGuiSlider* self, void* /*edx*/, char value)
{
    self->m_bConsole = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x72CBE0, CGuiSlider_SetConsole, "auto");
