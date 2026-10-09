#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the FSM's current state id. States 8 and 9 are returned as-is.
// For any other state, the virtual slot 0x134 is called with (8, 0) and its result is returned.
static int __fastcall CCharWeaponFsm_GetResolvedState(CCharWeaponFsm* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_000c);
    if (result != 9 && result != 8)
    {
        return self->vSlot134(8, 0);
    }
    return result;
}

HOOK(0x004807D0, CCharWeaponFsm_GetResolvedState, "auto");
