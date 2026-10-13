#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the index (0..9) of the first slot whose flag byte is zero, or -1.
// If the object's global-enable byte (+0x1708) is set, it instead sets +0x1704 to 1 and returns -1.
static int __fastcall S_00517800_QueryFirstFreeSlot(S_00517800* self, void* /*edx*/) {
    if (self->field_1708 != 0) {
        self->field_1704 = 1;
        return -1;
    }

    int result = -1;
    for (int slot = 0; slot < 10 && result == -1; ++slot) {
        // Slots are 0x24C bytes apart, starting at +0x14.
        uint32_t slotOffset = 0x14 + static_cast<uint32_t>(slot) * 0x24C;
        if (CSF_AT(uint8_t, self, slotOffset) == 0) {
            result = slot;
        }
    }
    return result;
}

HOOK(0x00517800, S_00517800_QueryFirstFreeSlot, "auto");
