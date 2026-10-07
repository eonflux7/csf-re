#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiButton setter (BOTON): stores the low byte argument at +0x1EC and returns it.
// Original: push ebp; mov ebp, esp; mov al, [ebp+8]; mov [ecx+0x1EC], al; pop ebp; ret 4
// (only al is written back, so the return type is a genuine byte/char, not a zero-extended int).
static char __fastcall CGuiButton_SetField01EC(CGuiButton* self, void* /*edx*/, char value)
{
    self->field_01ec = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x725580, CGuiButton_SetField01EC, "auto");
