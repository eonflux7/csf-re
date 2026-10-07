#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns whether bit 0x40 is set in the flags dword at offset 0x64.
static int __fastcall C_00860288_TestFlag40(C_00860288* self, void* /*edx*/)
{
    return (self->field_0064 & 0x40) == 0x40;
}

HOOK(0x6B29D0, C_00860288_TestFlag40, "auto");
