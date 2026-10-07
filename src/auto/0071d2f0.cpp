#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// C_00864508::SetFieldA1 -- stores the byte flag at +0xA1 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0xA1], al; ret 4  (only AL is defined on return).
static char __fastcall C_00864508_SetFieldA1(C_00864508* self, void* /*edx*/, char value)
{
    self->field_00a1 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x71D2F0, C_00864508_SetFieldA1, "auto");
