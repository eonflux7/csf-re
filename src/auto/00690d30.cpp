#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0085eac0: stores the byte argument at +0x118 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x118], al; ret 4 (returns the value in al only).
static char __fastcall C_0085eac0_SetByteFlag118(C_0085eac0* self, void* /*edx*/, char value)
{
    self->field_0118 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x690D30, C_0085eac0_SetByteFlag118, "auto");
