#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0083ac40: stores the byte flag at +0xE4 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0xE4], al; ret 4 (returns the value in al only).
static char __fastcall C_0083ac40_SetStateFlag(C_0083ac40* self, void* /*edx*/, char value)
{
    self->field_00e4 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x4CDE00, C_0083ac40_SetStateFlag, "auto");
