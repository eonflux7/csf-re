#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Game heap free (bound by address; the original calls this directly).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Frees the buffer pointer stored at +0x1C of every 44-byte graph record in
// [first, end). Each freed slot is cleared to 0. Returns the last value examined:
// the Mem_Free result if the last slot was freed, otherwise that slot's (zero)
// pointer value.
static int __cdecl WaypointGraph_FreeRecordBuffers(int first, int end, int /*unused a3*/, int /*unused a4*/)
{
    int result = 0;
    int record = first;

    while (record != end)
    {
        result = CSF_AT(int, record, 0x1C);
        if (result)
        {
            result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(int, record, 0x1C))));
            CSF_AT(int, record, 0x1C) = 0;
        }
        // Advance to the next 44-byte record.
        record += 0x2C;
    }
    return result;
}

HOOK(0x75D0E0, WaypointGraph_FreeRecordBuffers, "auto");
