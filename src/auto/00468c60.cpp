#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original growth routine for an S_0041e5f0 slot array. Direct call (not a vtable slot).
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Binds object id a2 into slot table a3 (or the shared table when bit 27 is set).
// a3 == -1 or a2 == 0 is a no-op and returns the caller's leftover value (passed as 0 here).
static int __fastcall SceneManager_BindObjectToId(S_0045d610* self, void* /*edx*/, int a2, int a3)
{
    int result = 0;

    if (a2 != 0 && a3 != -1)
    {
        S_0041e5f0* slotTable;
        if ((a3 & 0x8000000) != 0)
            slotTable = reinterpret_cast<S_0041e5f0*>(self->field_0078 + 16u);
        else
            slotTable = reinterpret_cast<S_0041e5f0*>(self->field_0078 + 16u * static_cast<uint32_t>(a3));

        // Unsigned comparison: the original uses jb.
        if (static_cast<uint32_t>(slotTable->field_0008) >= static_cast<uint32_t>(slotTable->field_0004))
            GrowButtonArray(slotTable, nullptr, slotTable->field_000c);

        uint32_t index = slotTable->field_0008;
        int* slot = reinterpret_cast<int*>(slotTable->field_0000 + 4u * index);
        result = static_cast<int>(index + 1u);
        slotTable->field_0008 = static_cast<uint32_t>(result);
        *slot = a2;
    }

    return result;
}

HOOK(0x468C60, SceneManager_BindObjectToId, "auto");
