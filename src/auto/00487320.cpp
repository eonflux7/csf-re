#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class lookup (CCharWeaponFsm side). Returns a nonzero result if it matched.
CSF_FUNC(int, __fastcall, LookupWeaponFsmHandle, 0x00481180, (void* self, void* edx, int a2));

// Indexed slot lookup. Returns the slot index into field_0078, or -1 when not found.
CSF_FUNC(int, __fastcall, LookupIndexedSlot, 0x004859D0, (void* self, void* edx, int a2));

// Resolves a handle object to its stored value pointer and reads the value.
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

// True when the handle object at `handle` currently holds the value `expected`.
static bool HandleHoldsValue(uint32_t handle, int expected)
{
    void* valuePointer = reinterpret_cast<void*>(Handle_GetValuePointer(reinterpret_cast<void*>(handle), nullptr));
    return *reinterpret_cast<int*>(valuePointer) == expected;
}

// Finds the action handle whose stored value equals a2.
// Order: base lookup, indexed slot table, then the three handle fields (last match wins,
// except field_008c which returns immediately).
static int __fastcall CCharActionFsm_FindActionByHandleValue(CCharActionFsm* self, void* /*edx*/, int a2)
{
    int result = LookupWeaponFsmHandle(self, nullptr, a2);
    if (result)
        return result;

    int slotIndex = LookupIndexedSlot(self, nullptr, a2);
    if (slotIndex != -1)
    {
        result = *reinterpret_cast<int*>(self->field_0078 + 4 * slotIndex);
        if (result)
            return result;
    }

    if (self->field_0084 && HandleHoldsValue(self->field_0084, a2))
        result = static_cast<int>(self->field_0084);

    if (self->field_0088 && HandleHoldsValue(self->field_0088, a2))
        result = static_cast<int>(self->field_0088);

    if (self->field_008c && HandleHoldsValue(self->field_008c, a2))
        return static_cast<int>(self->field_008c);

    return result;
}

HOOK(0x487320, CCharActionFsm_FindActionByHandleValue, "auto");
