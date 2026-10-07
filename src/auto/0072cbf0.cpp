#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiSlider::SetWrap (ENVOLVER) -- stores the wrap-enable byte at +0x16B and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x16B], al; ret 4  (returns the value in al only).
static char __fastcall CGuiSlider_SetWrap(CGuiSlider* self, void* /*edx*/, char value)
{
    self->m_bWrap = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x72CBF0, CGuiSlider_SetWrap, "auto");

