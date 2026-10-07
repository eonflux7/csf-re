#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiPanel flag setter at +0x18A.
// Original: push ebp / mov ebp, esp / mov al, [ebp+8] / mov [ecx+0x18A], al / pop ebp / ret 4
// Stores the passed byte into field_018a and returns it (al only).
static char __fastcall CGuiPanel_SetFlag018A(CGuiPanel* self, void* /*edx*/, char value)
{
    self->field_018a = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x72BD00, CGuiPanel_SetFlag018A, "auto");
