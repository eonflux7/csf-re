#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the byte at +0x2C unchanged when it is zero. Otherwise it forwards
// to the virtual slot 0x134 with (5, 0) and returns that result.
static char __fastcall CCharWeaponFsm_DispatchIfFlagged(CCharWeaponFsm* self, void* /*edx*/) {
    char flagValue = self->field_002c;
    if (flagValue) {
        return static_cast<char>(self->vSlot134(5, 0));
    }
    return flagValue;
}

HOOK(0x480CE0, CCharWeaponFsm_DispatchIfFlagged, "auto");
