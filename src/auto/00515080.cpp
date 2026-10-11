#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl ProfilesSavegames_ResetEntryRange(int firstEntry, int endEntry)
{
    uint32_t cursor = static_cast<uint32_t>(firstEntry);
    if (firstEntry != endEntry) {
        uint8_t* buffers = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(cursor)) + 56;

        do {
            if (CSF_AT(uint32_t, buffers, 0) != 0) {
                Mem_FreeDebug(reinterpret_cast<void*>(
                    static_cast<uintptr_t>(CSF_AT(uint32_t, buffers, 0))));
            }
            CSF_AT(uint32_t, buffers, 4) = 0;
            CSF_AT(uint32_t, buffers, 0) = 0;
            CSF_AT(uint32_t, buffers, 8) = 0;

            if (CSF_AT(uint32_t, buffers, -32) >= 8u) {
                if (CSF_AT(uint32_t, buffers, -52) != 0) {
                    Mem_FreeDebug(reinterpret_cast<void*>(
                        static_cast<uintptr_t>(CSF_AT(uint32_t, buffers, -52))));
                }
            }
            CSF_AT(uint32_t, buffers, -32) = 7;
            CSF_AT(uint32_t, buffers, -36) = 0;
            CSF_AT(uint16_t, buffers, -52) = 0;

            cursor += 80;
            buffers += 80;
        } while (cursor != static_cast<uint32_t>(endEntry));
    }
    return endEntry;
}

HOOK(0x00515080, ProfilesSavegames_ResetEntryRange, "auto");
