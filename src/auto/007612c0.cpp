#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, IsSubStateEnabled, 0x00760F90, (int a1));
CSF_FUNC(int, __cdecl, IsSubObjectFieldFlagSet, 0x00760FF0, (int a1));

// Returns non-zero when the weapon fire mode is not one of the excluded
// special modes (9, 32, 15), and neither sub-object flag nor sub-state
// check reports it as special.
static int __cdecl WeaponDefs_IsFireModeSpecial(int a1) {
    if (a1 == 9 || a1 == 0x20 || a1 == 0xF) {
        return 0;
    }
    if (IsSubObjectFieldFlagSet(a1) != 0) {
        return 0;
    }
    if (IsSubStateEnabled(a1) != 0) {
        return 0;
    }
    return 1;
}

HOOK(0x7612C0, WeaponDefs_IsFireModeSpecial, "auto");
