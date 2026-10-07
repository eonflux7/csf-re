#include "hook.h"
#include "types/auto_classes.h"

// Returns bit 4 of field_0058 (a packed flags word) as 0 or 1.
static int __fastcall C_00861e98_IsFlag0058Bit4Set(C_00861e98* self, void* /*edx*/)
{
    return (self->field_0058 >> 4) & 1;
}

HOOK(0x006E8530, C_00861e98_IsFlag0058Bit4Set, "auto");
