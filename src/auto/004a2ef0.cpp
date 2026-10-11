#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns true when the slot at (self + 0x0C + slot * 16) is empty.
// Slot 0 is empty only when the 0x1C and 0x2C fields are also zero.
// Slots 1 and 2 are checked against the 0x0C field.
// Any other index with a zero value also reports empty.
static bool __fastcall C_008344f0_IsSlotEmpty(C_008344f0* self, void* /*edx*/, int slot)
{
    uint32_t slotValue = CSF_AT(uint32_t, self, 0x0C + slot * 16);
    bool result = (slotValue == 0);

    if (slotValue == 0)
    {
        if (slot == 0)
        {
            return self->field_001c == 0 && self->field_002c == 0;
        }
        if (slot > 0 && slot <= 2)
        {
            return self->field_000c == 0;
        }
    }
    return result;
}

HOOK(0x4A2EF0, C_008344f0_IsSlotEmpty, "auto");
