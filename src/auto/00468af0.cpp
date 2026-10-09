#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Game heap free (bound by address; the original calls this directly).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees every non-null pointer in the table [first, end). Entries are 16 bytes
// apart (the pointer is the first dword of each 16-byte record). Each freed slot
// is cleared to 0. Returns the last value examined: the Mem_Free result if the
// last slot was freed, otherwise that slot's (zero) pointer value.
static int __cdecl MissionScript_FreeSlotRange(int* first, int* end, int /*unused a3*/, int /*unused a4*/)
{
    int result = 0;
    int* slot = first;

    while (slot != end)
    {
        result = *slot;
        if (*slot)
        {
            result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(*slot)));
            *slot = 0;
        }
        // Advance to the next 16-byte record.
        slot = reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(slot) + 0x10);
    }
    return result;
}

HOOK(0x468AF0, MissionScript_FreeSlotRange, "auto");
