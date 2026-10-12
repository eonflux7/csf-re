#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original base-class routine; called with this + two stack args, ret 8.
CSF_FUNC(int, __fastcall, BaseClass_ProcessWithArgs, 0x0049D060, (void* self, void* edx, int* a2, int a3));

// 0x49E1E0: forwards (a2, a3) to the base routine, sets the byte at +0x1B9 to 1, returns the base result.
static int __fastcall C_00834198_ProcessAndSetFlag1B9(C_00834198* self, void* /*edx*/, int* a2, int a3)
{
    int result = BaseClass_ProcessWithArgs(self, nullptr, a2, a3);
    self->field_01b9 = 1;
    return result;
}

HOOK(0x0049E1E0, C_00834198_ProcessAndSetFlag1B9, "auto");
