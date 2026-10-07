#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x006C9FD0: sets the byte field at offset 0x0C to zero.
static void __fastcall C_00860a30_ClearField000c(C_00860a30* self, void* /*edx*/, int /*a2*/)
{
    self->field_000c = 0;
}

HOOK(0x006C9FD0, C_00860a30_ClearField000c, "auto");
