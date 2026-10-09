#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the number of entries in the [field_0018, field_001c) range, in 4-byte
// units. If the index is below that count (unsigned compare) and the range is
// non-empty, forwards the index to virtual slot 0xF8; otherwise returns the count.
static unsigned int __fastcall CCharWeaponFsm_ResolveIndexedSlot(CCharWeaponFsm* self, void* /*edx*/, unsigned int slotIndex)
{
    unsigned int result = 0;

    uint32_t rangeBegin = self->field_0018;
    if (rangeBegin != 0)
    {
        // Signed difference shifted right by 2 (sar), as in the original.
        int32_t byteSpan = static_cast<int32_t>(self->field_001c - rangeBegin);
        result = static_cast<unsigned int>(byteSpan >> 2);

        if (slotIndex < result)
            return static_cast<unsigned int>(self->vSlotF8(static_cast<int>(slotIndex)));
    }

    return result;
}

HOOK(0x480D90, CCharWeaponFsm_ResolveIndexedSlot, "auto");
