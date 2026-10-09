#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Growth helper: grows the slot array at this->field_004c (ret 4, __thiscall).
CSF_FUNC(int, __fastcall, sub_522D00, 0x00522D00, (void* self, void* edx, int a2));

// Each slot is 12 bytes: [0] flag dword (0 = free), [4] float -1.0f, [8] dword 0.
// Returns the index of the first free slot, or appends a new slot and returns
// the previous element count.
static unsigned int __fastcall AcquireFreeSoundSlot(S_00521df0* self, void* /*edx*/)
{
    const uint32_t originalCount = self->field_0054;
    uint32_t index = 0;

    // Scan for an existing free slot (first byte zero).
    if (originalCount != 0) {
        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_004c);
        while (index < originalCount) {
            if (*entry == 0) {
                if (index != 0xFFFFFFFFu) {
                    return index;
                }
                break;
            }
            ++index;
            entry += 12;
        }
    }

    // No free slot: grow if full (unsigned compare), then append.
    if (self->field_0054 >= CSF_AT(uint32_t, self, 0x50)) {
        sub_522D00(&self->field_004c, nullptr, CSF_AT(int, self, 0x58));
    }

    // Re-read array base and count after a possible reallocation.
    const uint32_t slotIndex = self->field_0054;
    uint8_t* base = reinterpret_cast<uint8_t*>(self->field_004c);
    uint32_t* slot = reinterpret_cast<uint32_t*>(base + static_cast<size_t>(slotIndex) * 12u);
    self->field_0054 = slotIndex + 1;

    slot[0] = 0u;            // flag: byte 0 = 0 (upper bytes zeroed)
    slot[1] = 0xBF800000u;   // float -1.0f
    slot[2] = 0u;

    return originalCount;
}

HOOK(0x00523220, AcquireFreeSoundSlot, "auto");
