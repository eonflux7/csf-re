#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class routine (__thiscall, ret 0 -> pops nothing).
CSF_FUNC(unsigned int, __fastcall, sub_4D0400, 0x004D0400, (void* self, void* edx));

// Runs the base reset, then clears the dword at +0x70 to 0 and returns the base result.
static int __fastcall C_0083ae28_BaseResetAndClearField70(C_0083ae28* self, void* /*edx*/)
{
    int result = static_cast<int>(sub_4D0400(self, nullptr));
    self->field_0070 = 0;
    return result;
}

HOOK(0x004CEBE0, C_0083ae28_BaseResetAndClearField70, "auto");
