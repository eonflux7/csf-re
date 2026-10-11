#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Mem_FreeDebug: game heap free used by this array's element cleanup.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets 32 spawn-point slots (56-byte records) starting at this+0x71C, freeing any
// owned buffer whose tag is >= 8. Each slot is reset to tag 7, field -4 = 0, word -0x14 = 0.
static uint32_t __fastcall ResetSpawnPointSlots(void* self, void* /*edx*/) {
    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 0x71C;
    int remaining = 32;
    uint32_t result = 0;
    do {
        cursor -= 0x38;
        result = CSF_AT(uint32_t, cursor, 0);
        if (result >= 8) {
            result = CSF_AT(uint32_t, cursor, -0x14);
            if (result != 0) {
                result = static_cast<uint32_t>(
                    Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result))));
            }
        }
        --remaining;
        CSF_AT(uint32_t, cursor, 0) = 7;
        CSF_AT(uint32_t, cursor, -4) = 0;
        CSF_AT(uint16_t, cursor, -0x14) = 0;
    } while (remaining != 0);
    return result;
}

HOOK(0x5546C0, ResetSpawnPointSlots, "auto");
