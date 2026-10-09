#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the byte flag at +0x69 when it is clear. When it is set, forwards
// to the virtual slot 0x134 with (0, 0) and returns that result.
static char __fastcall CCharWeaponFsm_ReturnFlagOrCallSlot134(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char flag = static_cast<char>(self->field_0069);
    if (flag)
        return static_cast<char>(self->vSlot134(0, 0));
    return flag;
}

HOOK(0x0047FC20, CCharWeaponFsm_ReturnFlagOrCallSlot134, "auto");
