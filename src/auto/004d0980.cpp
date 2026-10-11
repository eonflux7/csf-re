#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class preparation routine (__thiscall, ret 0 -> pops nothing).
CSF_FUNC(int, __fastcall, TCharTask_PrepareBase, 0x004DB3F0, (void* self, void* edx));

// Runs the base preparation, then clears the dword at +0x44 to 0 and returns the base result.
static int __fastcall C_0083b770_ResetPreparedTaskState(C_0083b770* self, void* /*edx*/)
{
    int result = TCharTask_PrepareBase(self, nullptr);
    self->field_0044 = 0;
    return result;
}

HOOK(0x004D0980, C_0083b770_ResetPreparedTaskState, "auto");
