#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Searches the three 16-byte slots starting at +0x0C for the value a2.
// Returns the slot index (0..2) of the first match, or -1 if none matches.
// A slot holding 0 never matches.
static int __fastcall C_008344f0_FindSlotIndex(C_008344f0* self, void* /*edx*/, int searchValue)
{
    const uint32_t kFirstSlotOffset = 0x0C;
    const uint32_t kSlotStride = 0x10;
    const int kSlotCount = 3;

    int slotIndex = 0;
    for (;;) {
        uint32_t slotOffset = kFirstSlotOffset + static_cast<uint32_t>(slotIndex) * kSlotStride;
        uint32_t slotValue = CSF_AT(uint32_t, self, slotOffset);
        if (slotValue != 0 && slotValue == static_cast<uint32_t>(searchValue)) {
            return slotIndex;
        }
        if (static_cast<unsigned int>(++slotIndex) >= static_cast<unsigned int>(kSlotCount)) {
            return -1;
        }
    }
}

HOOK(0x4A2C80, C_008344f0_FindSlotIndex, "auto");
