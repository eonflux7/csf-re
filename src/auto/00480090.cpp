#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00480090, char __thiscall (CCharWeaponFsm* this, int a2), ret 4.
// If the byte at +0x69 is set, issues vSlot134(5, 7) when +0x60 is set, otherwise vSlot134(5, 0).
// Otherwise returns the (zero) +0x69 byte without calling anything.
static char __fastcall CCharWeaponFsm_TriggerActionIfFlagged(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char result = static_cast<char>(self->field_0069);
    if (result)
    {
        if (self->field_0060)
            return static_cast<char>(self->vSlot134(5, 7));
        else
            return static_cast<char>(self->vSlot134(5, 0));
    }
    return result;
}

HOOK(0x480090, CCharWeaponFsm_TriggerActionIfFlagged, "auto");
