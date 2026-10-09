#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the low byte of eax as a 0/1 flag: true when the object is not in the
// "idle for slot a2" state (or its cached value differs from field_03ec). The
// upper 24 bits of the result are the original field_01c4 value, left untouched.
static int __fastcall CPlayerCharacter_QueryDirtyFlag(CPlayerCharacter* self, void* /*edx*/, int slotId)
{
    bool stillPending = true;

    if (self->field_02e0 == static_cast<uint32_t>(slotId) && !self->field_03dd)
    {
        uint32_t listBegin = self->field_0300;
        if (listBegin == 0 ||
            ((static_cast<int>(self->field_0304 - listBegin)) >> 3) == 0)
        {
            uint32_t otherBegin = self->field_0310;
            if (otherBegin == 0 ||
                ((static_cast<int>(self->field_0314 - otherBegin)) >> 3) == 0)
            {
                stillPending = false;
            }
        }
    }

    uint32_t cachedValue = static_cast<uint32_t>(CSF_AT(int, self, 0x1C4));
    bool changed = stillPending || (cachedValue != self->field_03ec);

    // LOBYTE(result) = flag; upper bytes of eax keep field_01c4.
    uint32_t packed = (cachedValue & 0xFFFFFF00u) | (changed ? 1u : 0u);
    return static_cast<int>(packed);
}

HOOK_KEEP_ECX(0x4968D0, CPlayerCharacter_QueryDirtyFlag, 1, "auto");
