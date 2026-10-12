#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards three stored fields of this CPlayerCharacter (+0x39C if nonzero, then +0x22C,
// then +0x16C) to the target object's vtable slot 0xA0, passing a3 and 0 as the trailing
// arguments. Returns the result of the final call.
static int __fastcall CPlayerCharacter_DispatchSlotsToTarget(CPlayerCharacter* self, void* /*edx*/, void* target, int a3)
{
    uint32_t slotValue = self->field_039c;
    if (slotValue)
    {
        ((void (__fastcall*)(void*, void*, uint32_t, int, int))CSF_AT(void**, target, 0)[0xA0 / 4])(
            target, nullptr, slotValue, a3, 0);
    }

    ((void (__fastcall*)(void*, void*, uint32_t, int, int))CSF_AT(void**, target, 0)[0xA0 / 4])(
        target, nullptr, self->field_022c, a3, 0);

    return ((int (__fastcall*)(void*, void*, uint32_t, int, int))CSF_AT(void**, target, 0)[0xA0 / 4])(
        target, nullptr, self->field_016c, a3, 0);
}

HOOK(0x4953F0, CPlayerCharacter_DispatchSlotsToTarget, "auto");
