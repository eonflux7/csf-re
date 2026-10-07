#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiPanel::SetField158 -- stores the byte argument at +0x158 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x158], al; ret 4  (result returned in al only).
static char __fastcall CGuiPanel_SetField158(CGuiPanel* self, void* /*edx*/, char value)
{
    self->field_0158 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x72BC60, CGuiPanel_SetField158, "auto");
