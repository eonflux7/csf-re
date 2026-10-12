#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Game heap free (bound by address; the original calls this directly).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees every non-null pointer in the table [first, end). Records are 28 bytes
// apart and the pointer is the dword at +0x0C of each record. Each freed slot is
// cleared to 0. Returns the last value examined: the Mem_Free result if the last
// slot was freed, otherwise that slot's (zero) pointer value.
static int __cdecl EntryTable_FreePointerRange(int first, int end, int /*unused a3*/, int /*unused a4*/)
{
    int result = 0;
    uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(first)));
    uint8_t* recordEnd = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(end)));

    while (record != recordEnd)
    {
        result = CSF_AT(int, record, 0x0C);
        if (CSF_AT(int, record, 0x0C))
        {
            result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(CSF_AT(int, record, 0x0C)))));
            CSF_AT(int, record, 0x0C) = 0;
        }
        // Advance to the next 28-byte record.
        record += 0x1C;
    }
    return result;
}

HOOK(0x40B480, EntryTable_FreePointerRange, "auto");
