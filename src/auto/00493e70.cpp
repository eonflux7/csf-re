#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00493E70, __thiscall, no stack args, ret 0.
// Returns the stored-target state word; when it is still zero and the
// stored slot index at +0x2DC is non-negative, latches the state to 1 and
// copies the stored target values into +0x2A0 / +0x2A4.
static int __fastcall CPlayerCharacter_SelectStoredTarget(CPlayerCharacter* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0298);
    if (result == 0)
    {
        result = static_cast<int>(self->field_02dc);
        if (static_cast<int32_t>(self->field_02dc) >= 0)
        {
            self->field_0298 = 1;
            result = static_cast<int>(self->field_02d8);
            self->field_02a0 = static_cast<uint32_t>(result);
            self->field_02a4 = self->field_02dc;
        }
    }
    return result;
}

HOOK_KEEP_ECX(0x493E70, CPlayerCharacter_SelectStoredTarget, 0, "auto");
