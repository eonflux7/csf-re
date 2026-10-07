#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns whether the dword at offset 0x88 is non-zero.
static int __fastcall C_00860288_IsField0088Set(C_00860288* self, void* /*edx*/)
{
    return self->field_0088 != 0;
}

HOOK(0x6B2A90, C_00860288_IsField0088Set, "auto");
