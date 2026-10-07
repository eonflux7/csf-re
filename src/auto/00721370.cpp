#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// CGuiEdit::SetFlag0148 -- stores the byte flag passed on the stack at +0x148 and
// returns it (the original only defines AL; the caller compares/uses the byte).
// Original: mov al, [ebp+8]; mov [ecx+0x148], al; ret 4
static char __fastcall CGuiEdit_SetFlag0148(CGuiEdit* self, void* /*edx*/, char value)
{
    self->field_0148 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x721370, CGuiEdit_SetFlag0148, "auto");
