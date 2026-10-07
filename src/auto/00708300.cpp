#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Returns bit 4 of the value stored at offset 0x14.
// Original: mov eax,[ecx+0x14]; shr eax,4; and eax,1; ret 4
static int __fastcall C_00863430_GetField14Bit4(C_00863430* self, void* /*edx*/, int /*unused*/)
{
    return static_cast<int>((self->field_0014 >> 4) & 1u);
}

HOOK(0x00708300, C_00863430_GetField14Bit4, "auto");
