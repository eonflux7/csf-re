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

CSF_FUNC(void, __fastcall, CorePaths_ReleaseOwnedPathSlot, 0x00693B40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Frees every owned path slot (inline array at +0x18 for the first 10 entries,
// then a secondary table pointed to by +0x08), clears the +0x04 flag and frees the
// table pointer at +0x08. Returns the result of the last free, or 0.
static int __fastcall CorePaths_ClearSlotState(void* self, void* /*edx*/)
{
    const uint32_t slotCount = CSF_AT(uint32_t, self, 0x40) + CSF_AT(uint32_t, self, 0x10);

    if (slotCount != 0) {
        for (uint32_t i = 0; i < slotCount; ++i) {
            int slot;
            if (i >= 0xA) {
                // Secondary table: re-read the table pointer each time, as the original does.
                uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uintptr_t, self, 0x08));
                slot = *reinterpret_cast<int*>(table + (i * 4 - 40));
            } else {
                slot = CSF_AT(int, self, 0x18 + i * 4);
            }

            if (slot) {
                CorePaths_ReleaseOwnedPathSlot(reinterpret_cast<void*>(static_cast<uintptr_t>(slot)), nullptr);
                CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(slot)));
            }
        }
    }

    if (CSF_AT(int, self, 0x04) != 0) {
        CSF_AT(int, self, 0x04) = 0;
    }

    int result = CSF_AT(int, self, 0x08);
    if (result) {
        result = CoreMemory_Free(reinterpret_cast<void*>(CSF_AT(uintptr_t, self, 0x08)));
        CSF_AT(int, self, 0x08) = 0;
    }
    return result;
}

HOOK(0x00693370, CorePaths_ClearSlotState, "auto");
