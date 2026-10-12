#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a range of 0x3C-byte clump dictionary entries, freeing their owned buffers.
// Entries are walked from begin to end (exclusive) with a 0x3C-byte stride.
static int __cdecl GfxClumpDict_FreeEntryRange(int* begin, int* end)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(begin);
    uint8_t* last = reinterpret_cast<uint8_t*>(end);
    int result = 0;

    while (entry != last)
    {
        // Entry +0x20: heap block allocated with Mem_Alloc; free it and clear the slot.
        int ownedBlock = CSF_AT(int, entry, 0x20);
        if (ownedBlock != 0)
        {
            result = Mem_Free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(ownedBlock)));
            CSF_AT(int, entry, 0x20) = 0;
        }

        // Entry +0x18: capacity. Values >= 0x10 mean +0x04 points to a separately allocated buffer.
        if (static_cast<unsigned int>(CSF_AT(int, entry, 0x18)) >= 0x10)
        {
            int debugBlock = CSF_AT(int, entry, 0x04);
            if (debugBlock != 0)
            {
                result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<std::uintptr_t>(debugBlock)));
            }
        }

        CSF_AT(int, entry, 0x18) = 15;
        CSF_AT(int, entry, 0x14) = 0;
        CSF_AT(uint8_t, entry, 0x04) = 0;

        entry += 0x3C;
    }
    return result;
}

HOOK(0x006D4B40, GfxClumpDict_FreeEntryRange, "auto");
