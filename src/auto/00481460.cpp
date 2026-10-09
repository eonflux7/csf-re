#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the int stored at table[index] (4-byte entries), where table = field_0018
// and index = field_0024. Returns 0 when index is -1 (no entry selected).
static int __fastcall CCharWeaponFsm_GetIndexedEntryOrZero(CCharWeaponFsm* self, void* /*edx*/)
{
    uint32_t entryIndex = self->field_0024;
    int result = 0;
    if (entryIndex != static_cast<uint32_t>(-1)) {
        uint32_t entryTable = self->field_0018;
        result = *reinterpret_cast<int*>(entryTable + 4u * entryIndex);
    }
    return result;
}

HOOK(0x00481460, CCharWeaponFsm_GetIndexedEntryOrZero, "auto");
