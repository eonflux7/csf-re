#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

// Returns the current item count when the action is allowed; otherwise forwards
// to the slot-0x134 handler with (14, 0) to cancel/redirect the action.
static int __fastcall CCharActionFsm_CheckItemsOrCancelAction(CCharActionFsm* self, void* /*edx*/)
{
    bool hasNoWeaponFlagSet = self->vIsField000cNull();
    uint32_t weaponStateKind = self->m_stateCode;

    if (weaponStateKind == 3)
        return self->vBeginWeaponStateChange(14, 0);
    if (weaponStateKind == 16)
        return self->vBeginWeaponStateChange(14, 0);

    C_0086754c* itemContainer = (C_0086754c*)self->vGetIndexedEntryOrZero();
    int currentItemCount = GetCurrentItemCount(itemContainer, nullptr);

    if (!currentItemCount || hasNoWeaponFlagSet)
        return self->vBeginWeaponStateChange(14, 0);

    return currentItemCount;
}

HOOK(0x00483640, CCharActionFsm_CheckItemsOrCancelAction, "auto");
