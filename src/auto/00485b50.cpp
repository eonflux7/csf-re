#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the int stored at table[index] (4-byte entries), where table = field_0078
// and index = field_0094. Returns 0 when index is -1 (no entry selected).
static int __fastcall CCharActionFsm_GetIndexedEntryOrZero(CCharActionFsm* self, void* /*edx*/)
{
    uint32_t entryIndex = self->field_0094;
    int result = 0;
    if (entryIndex != static_cast<uint32_t>(-1)) {
        uint32_t entryTable = self->field_0078;
        result = *reinterpret_cast<int*>(entryTable + 4u * entryIndex);
    }
    return result;
}

HOOK(0x00485B50, CCharActionFsm_GetIndexedEntryOrZero, "auto");
