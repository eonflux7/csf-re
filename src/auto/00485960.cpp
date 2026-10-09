#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the int stored at table[slot] (4-byte entries), where table = field_0078
// and slot = field_0090. Returns 0 when slot is -1 (no entry selected).
static int __fastcall CCharActionFsm_ResolveTargetFromSlot(CCharActionFsm* self, void* /*edx*/)
{
    uint32_t slotIndex = self->field_0090;
    int result = 0;
    if (slotIndex != static_cast<uint32_t>(-1)) {
        uint32_t slotTable = self->field_0078;
        result = *reinterpret_cast<int*>(slotTable + 4u * slotIndex);
    }
    return result;
}

HOOK(0x00485960, CCharActionFsm_ResolveTargetFromSlot, "auto");
