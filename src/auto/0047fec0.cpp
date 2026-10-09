#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the weapon-FSM flag byte. When the flag is set, it dispatches
// virtual slot 0x138 with state 3 and returns that result instead.
static char __fastcall CCharWeaponFsm_DispatchStateIfFlagged(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char flagValue = static_cast<char>(self->field_0069);
    if (flagValue)
    {
        return static_cast<char>(self->vSlot138(3));
    }
    return flagValue;
}

HOOK(0x47FEC0, CCharWeaponFsm_DispatchStateIfFlagged, "auto");
