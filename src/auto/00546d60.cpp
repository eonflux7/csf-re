#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// Game heap free (0x691110): the original calls this for string buffers that
// have spilled past the 16-byte small-string buffer.
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* block));

// Resets a contiguous range of 68-byte menu-extras records. The original walks
// with a cursor (esi) that starts at record+0x40 and addresses both string
// members by negative displacements from that cursor. For each string whose
// capacity is at least 16, the heap buffer is freed. Then each string is set
// to empty with capacity 15 and its first byte cleared. Walks from `first` up
// to `last` in steps of 68 bytes. Returns `last`.
static int __cdecl MenuExtras_ResetStringRange(int first, int last)
{
    int cursor = first;

    if (first != last)
    {
        do
        {
            // esi = record + 0x40 in the original; all accesses are relative to it.
            uint8_t* esi = reinterpret_cast<uint8_t*>(cursor) + 0x40;

            // First string (capacity at [esi], length at [esi-4], first byte at [esi-0x14]).
            if (CSF_AT(uint32_t, esi, 0) >= 0x10u)
                Mem_Free(CSF_AT(void*, esi, -0x14));
            CSF_AT(uint32_t, esi, 0) = 15;
            CSF_AT(uint32_t, esi, -4) = 0;
            CSF_AT(uint8_t, esi, -0x14) = 0;

            // Second string (capacity at [esi-0x24], length at [esi-0x28], first byte at [esi-0x38]).
            if (CSF_AT(uint32_t, esi, -0x24) >= 0x10u)
                Mem_Free(CSF_AT(void*, esi, -0x38));
            CSF_AT(uint32_t, esi, -0x24) = 15;
            CSF_AT(uint32_t, esi, -0x28) = 0;
            CSF_AT(uint8_t, esi, -0x38) = 0;

            cursor += 68;
        } while (cursor != last);
    }
    return last;
}

HOOK(0x546D60, MenuExtras_ResetStringRange, "auto");
