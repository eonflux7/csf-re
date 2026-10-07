#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Zeroes the dword at offset 0x8 of the object.
static void __fastcall C_0082d58c_ResetField0008(C_0082d58c* self, void* /*edx*/)
{
    self->field_0008 = 0;
}

HOOK(0x409290, C_0082d58c_ResetField0008, "auto");
