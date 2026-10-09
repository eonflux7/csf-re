#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the pending value in field_0028. If one is pending (not -1), it is
// passed to the vSlotF8 virtual and the slot is cleared to -1. The return value
// is the virtual's result in that case, otherwise the pending value (-1).
static int __fastcall CCharWeaponFsm_ConsumePendingSlot(CCharWeaponFsm* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0028);
    if (result != -1)
    {
        result = self->vSetCurrentWeaponAction(result);
        self->field_0028 = 0xFFFFFFFFu;
    }
    return result;
}

HOOK(0x480CC0, CCharWeaponFsm_ConsumePendingSlot, "auto");
