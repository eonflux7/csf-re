#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class reset routine (__thiscall, ret 0 -> pops nothing).
CSF_FUNC(int, __fastcall, C_0083b958_ResetBase, 0x004D1700, (void* self, void* edx));

// Runs the base reset, then clears the dword at +0x44 to 0 and returns the base result.
static int __fastcall C_0083b770_ResetBaseAndClearField0044(C_0083b770* self, void* /*edx*/)
{
    int result = C_0083b958_ResetBase(self, nullptr);
    self->field_0044 = 0;
    return result;
}

HOOK(0x004D0960, C_0083b770_ResetBaseAndClearField0044, "auto");
