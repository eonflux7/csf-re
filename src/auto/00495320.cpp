#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, InvokeVtbl134WithZeroArgs, 0x00482830, (void* self, void* edx));

static int __fastcall CPlayerCharacter_ResetFsmFlagsAndCounter(CPlayerCharacter* self, void* /*edx*/)
{
    void* actionFsm = reinterpret_cast<void*>(self->field_0044);
    int result = InvokeVtbl134WithZeroArgs(actionFsm, nullptr);
    self->field_02e0 &= ~0x8000u;
    self->field_02e4 = 0;
    return result;
}

HOOK(0x00495320, CPlayerCharacter_ResetFsmFlagsAndCounter, "auto");
