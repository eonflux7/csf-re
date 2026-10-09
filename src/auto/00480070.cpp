#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the weapon-FSM flag byte. When the flag is nonzero, dispatches
// virtual slot 0x138 with state 8 and returns that result instead.
static char __fastcall CCharWeaponFsm_ReturnFlagOrDispatchSlot8(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char flagValue = static_cast<char>(self->field_0069);
    if (flagValue)
    {
        return static_cast<char>(self->vSetStateAndDispatch(8));
    }
    return flagValue;
}

HOOK(0x480070, CCharWeaponFsm_ReturnFlagOrDispatchSlot8, "auto");
