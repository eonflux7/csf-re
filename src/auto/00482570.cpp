#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original fallback path (tail-jumped to when the action is disabled or the counter is not positive).
CSF_FUNC(int, __fastcall, CCharActionFsm_FallbackDispatch, 0x0047FC40, (void* self, void* edx, int a2));

// Dispatches a special action through the vtable slot at +0x134, choosing the argument pair
// from the current state (state 8 uses (5, 7), otherwise (5, 0)). Falls back to the base
// implementation when the action flag is clear or the counter is not positive.
static int __fastcall CCharActionFsm_DispatchActionOrFallback(CCharActionFsm* self, void* /*edx*/, int a2)
{
    if (!self->field_00bd || static_cast<int>(self->field_00c0) <= 0)
    {
        // Tail jump: forward the caller's stack argument unchanged.
        return CCharActionFsm_FallbackDispatch(self, nullptr, a2);
    }

    if (self->field_0010 == 8)
    {
        return self->vBeginWeaponStateChange(5, 7);
    }

    return self->vBeginWeaponStateChange(5, 0);
}

HOOK(0x482570, CCharActionFsm_DispatchActionOrFallback, "auto");
