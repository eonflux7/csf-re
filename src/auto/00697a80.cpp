#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap free (Mem_FreeDebug), __cdecl, one argument, caller pops.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets a range of 32-byte mission script string records [first, last).
// Each record: +0x04 owned buffer pointer (low byte cleared after release),
// +0x14 length/counter = 0, +0x18 capacity = 15. If the capacity was >= 0x10
// and the buffer pointer was non-null, the buffer is freed.
// The return value mirrors the last eax value produced by the loop.
static int __cdecl MissionScript_DestroyStringRange(int* first, int* last)
{
    int result = 0;
    for (int* record = first; record != last; record += 8)
    {
        // Unsigned compare: capacity >= 0x10 means the buffer is heap-allocated.
        if (static_cast<unsigned int>(record[6]) >= 0x10)
        {
            result = record[1];
            if (result)
                result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(record[1])));
        }
        record[6] = 15;
        record[5] = 0;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(record) + 4) = 0;  // byte at +4
    }
    return result;
}

HOOK(0x00697A80, MissionScript_DestroyStringRange, "auto");
