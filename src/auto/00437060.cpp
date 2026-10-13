#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CGuiControl: stores the byte flag at +0x130 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x130], al; ret 4 (returns the value in al only).
static char __fastcall CGuiControl_SetStateFlag(CGuiControl* self, void* /*edx*/, char value)
{
    self->field_0130 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x437060, CGuiControl_SetStateFlag, "auto");
