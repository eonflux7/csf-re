#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiPanel::SetFlag0189 -- stores the byte flag at +0x189 and returns it (in al only).
// Original: push ebp; mov ebp, esp; mov al, [ebp+8]; mov [ecx+0x189], al; pop ebp; ret 4
static char __fastcall CGuiPanel_SetFlag0189(CGuiPanel* self, void* /*edx*/, char value)
{
    self->field_0189 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x72BCF0, CGuiPanel_SetFlag0189, "auto");

