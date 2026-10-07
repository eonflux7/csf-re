#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiListBoxBase::SetLines (LINEAS) -- stores the byte flag at +0x16C and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x16C], al; ret 4  (returns the value in al only).
static char __fastcall CGuiListBoxBase_SetLines(CGuiListBoxBase* self, void* /*edx*/, char value)
{
    self->m_bLines = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x7162C0, CGuiListBoxBase_SetLines, "auto");
