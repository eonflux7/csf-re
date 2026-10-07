#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns whether bit 7 (0x80) of field_0064 is set.
// Original: and al,0x80 / sub al,0x80 / neg al / sbb eax,eax / inc eax -> exact 0 or 1 in eax.
static int __fastcall C_00860288_IsFlag80Set(C_00860288* self, void* /*edx*/)
{
    return (self->field_0064 & 0x80) == 0x80;
}

HOOK(0x006B29E0, C_00860288_IsFlag80Set, "auto");
