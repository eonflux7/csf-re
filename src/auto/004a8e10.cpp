#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00835228: stores the byte flag at +0x98 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x98], al; ret 4 (returns the value in al only).
static char __fastcall C_00835228_SetStateFlag(C_00835228* self, void* /*edx*/, char value)
{
    self->field_0098 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x4A8E10, C_00835228_SetStateFlag, "auto");
