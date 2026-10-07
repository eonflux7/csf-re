#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiPanel flag setter at +0x188.
// Original: mov al, [ebp+8]; mov [ecx+0x188], al; ret 4  (returns the value in al only).
static char __fastcall CGuiPanel_SetFlag188(CGuiPanel* self, void* /*edx*/, char value)
{
    self->field_0188 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x72BCE0, CGuiPanel_SetFlag188, "auto");
