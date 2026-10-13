#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets every 100-byte entry in [startAddr, endAddr). Each entry's cursor points at
// its capacity field (entry + 0x58), matching the original's esi. Entries whose capacity
// has grown to 16 or more free their heap buffer first. Then the entry is marked empty
// with an inline capacity of 15, and its vtable is re-stamped.
static int __cdecl ResetEntryRange(int startAddr, int endAddr) {
    uintptr_t current = static_cast<uintptr_t>(static_cast<uint32_t>(startAddr));
    const uintptr_t end = static_cast<uintptr_t>(static_cast<uint32_t>(endAddr));

    if (current != end) {
        do {
            uint8_t* esi = reinterpret_cast<uint8_t*>(current + 0x58);

            if (CSF_AT(uint32_t, esi, 0) >= 0x10u) {
                void* buffer = CSF_AT(void*, esi, -0x14);
                if (buffer != nullptr) {
                    Mem_FreeDebug(buffer);
                }
            }

            CSF_AT(uint32_t, esi, 0) = 15;
            CSF_AT(uint32_t, esi, -0x4) = 0;
            CSF_AT(uint8_t, esi, -0x14) = 0;
            CSF_AT(void*, esi, -0x48) = reinterpret_cast<void*>(CSF_VTBL(0x0082D410));

            current += 100;
        } while (current != end);
    }

    return endAddr;
}

HOOK(0x0044BFC0, ResetEntryRange, "auto");
