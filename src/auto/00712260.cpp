#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AudioEnum_Dispatch, 0x007DF740, (int a1, int a2));

static int __fastcall C_00863dd0_ForwardToCallbackAndClearField0C(C_00863dd0* self, void* /*edx*/, int a2, int a3, int a4) {
    int result = AudioEnum_Dispatch((int)self->field_0004, a2);
    self->field_000c = 0;
    return result;
}

HOOK(0x00712260, C_00863dd0_ForwardToCallbackAndClearField0C, "auto");
