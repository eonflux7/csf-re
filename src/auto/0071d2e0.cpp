#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// C_00864508::SetByteFlagA0 -- stores the given byte at +0xA0 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0xA0], al; ret 4  (returns in al only).
static char __fastcall C_00864508_SetByteFlagA0(C_00864508* self, void* /*edx*/, char value)
{
    self->field_00a0 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x71D2E0, C_00864508_SetByteFlagA0, "auto");
