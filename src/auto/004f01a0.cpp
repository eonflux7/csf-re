#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, S_006b0320_ReleaseBuffers, 0x006B1010, (S_006b0320* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Releases the heap-owned buffers of each 0x68-byte text entry in [firstEntry, endEntry)
// and resets the entry to its empty state. Returns endEntry, as the original does.
static int __cdecl TextDatabase_ReleaseEntryRange(int firstEntry, int endEntry, int /*unused*/, int /*unused*/) {
    int result = endEntry;
    int currentEntry = firstEntry;
    if (currentEntry != endEntry) {
        do {
            uint8_t* rec = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(currentEntry));

            // Sub-object at +0x30 (the original passes esi+0xc, where esi = rec+0x24).
            S_006b0320_ReleaseBuffers(reinterpret_cast<S_006b0320*>(rec + 0x30), nullptr);

            if (CSF_AT(void*, rec, 0x24) != nullptr) {
                Mem_FreeDebug(CSF_AT(void*, rec, 0x24));
            }
            CSF_AT(uint32_t, rec, 0x24) = 0;
            CSF_AT(uint32_t, rec, 0x28) = 0;
            CSF_AT(uint32_t, rec, 0x2C) = 0;

            if (CSF_AT(uint32_t, rec, 0x1C) >= 0x10u) {
                if (CSF_AT(void*, rec, 0x08) != nullptr) {
                    Mem_FreeDebug(CSF_AT(void*, rec, 0x08));
                }
            }
            result = endEntry;
            CSF_AT(uint32_t, rec, 0x1C) = 15;
            CSF_AT(uint32_t, rec, 0x18) = 0;
            CSF_AT(uint8_t, rec, 0x08) = 0;

            currentEntry += 0x68;
        } while (currentEntry != endEntry);
    }
    return result;
}

HOOK(0x4F01A0, TextDatabase_ReleaseEntryRange, "auto");
