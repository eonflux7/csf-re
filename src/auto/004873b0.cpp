#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resolves a slot index through the action table (virtual lookup), falling back
// to the secondary lookup at 0x485A20 and reading the matching value table.
CSF_FUNC(int, __fastcall, sub_485A20, 0x00485A20, (void* self, void* edx, int a2));

static int __fastcall CCharActionFsm_GetResolvedSlotValue(CCharActionFsm* self, void* /*edx*/, int a2)
{
    int primarySlot = self->vFindSubObjectIndex(a2);
    if (primarySlot != -1)
    {
        uint32_t* primaryTable = reinterpret_cast<uint32_t*>(self->field_0018);
        return static_cast<int>(primaryTable[primarySlot]);
    }

    int fallbackSlot = sub_485A20(self, nullptr, a2);
    if (fallbackSlot == -1)
        return 0;

    uint32_t* fallbackTable = reinterpret_cast<uint32_t*>(self->field_0078);
    return static_cast<int>(fallbackTable[fallbackSlot]);
}

HOOK(0x004873B0, CCharActionFsm_GetResolvedSlotValue, "auto");
