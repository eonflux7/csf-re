#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the entry at index vSlotBC(a2) of the table at field_0018, or 0 when the lookup yields -1.
static int __fastcall CCharWeaponFsm_LookupEntryByIndexOrZero(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    int entryIndex = self->vFindSubObjectIndex(a2);
    if (entryIndex == -1)
        return 0;

    return *reinterpret_cast<int*>(self->field_0018 + 4 * entryIndex);
}

HOOK(0x004811C0, CCharWeaponFsm_LookupEntryByIndexOrZero, "auto");
