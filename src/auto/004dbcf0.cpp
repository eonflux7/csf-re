#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Clears a table of slot objects: calls the virtual slot 0 (argument 1) on each
// non-null entry, zeroes that entry, then resets the slot count to 0.
static int __fastcall AiState_PrepareSlot(uint32_t* self, void* /*edx*/) {
    uint32_t slotCount = CSF_AT(uint32_t, self, 0x1c);
    uint32_t index = 0;
    int result = 0;

    if (slotCount != 0) {
        do {
            // The table pointer is reloaded on every iteration.
            uint32_t tableBase = CSF_AT(uint32_t, self, 0x14);
            result = static_cast<int>(tableBase);

            uint32_t slotValue = CSF_AT(uint32_t, reinterpret_cast<void*>(tableBase), index * 4);
            if (slotValue != 0) {
                void* slotObj = reinterpret_cast<void*>(slotValue);
                // Virtual slot 0 of the slot object, called __thiscall(1).
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, slotObj, 0)[0])(slotObj, nullptr, 1);

                // The table pointer is reloaded again before clearing the entry.
                result = static_cast<int>(CSF_AT(uint32_t, self, 0x14));
                CSF_AT(uint32_t, reinterpret_cast<void*>(result), index * 4) = 0;
            }
            ++index;
        } while (index < slotCount);

        CSF_AT(uint32_t, self, 0x1c) = 0;
    } else {
        CSF_AT(uint32_t, self, 0x1c) = 0;
    }

    return result;
}

HOOK(0x004DBCF0, AiState_PrepareSlot, "auto");
