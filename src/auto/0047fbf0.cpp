#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: char __thiscall sub_47FBF0(CCharWeaponFsm* this, int a2), ret 4.
// a2 is never read by the original.
static char __fastcall CCharWeaponFsm_UpdateWhenFlagSet(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char flag = self->field_0069;
    if (flag == 0)
        return flag;

    self->vCallVirtualMethod1E8(2);
    return static_cast<char>(self->vSlot134(2, 0));
}

HOOK(0x0047FBF0, CCharWeaponFsm_UpdateWhenFlagSet, "auto");
