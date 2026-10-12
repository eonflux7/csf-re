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

// Resets a range of 52-byte index table entries [first, last).
// Each entry: +0x04 owned buffer pointer (low byte cleared after release),
// +0x14 length/counter = 0, +0x18 capacity = 15. If the capacity was >= 0x10
// and the buffer pointer was non-null, the buffer is freed.
// The two trailing arguments are part of the original cdecl signature and are unused.
// The return value mirrors the last eax value produced by the loop.
static int __cdecl LevelIndexTable_Destroy(int* first, int* last, int unusedA3, int unusedA4)
{
    (void)unusedA3;
    (void)unusedA4;

    int result = 0;
    for (int* entry = first; entry != last; entry += 13)
    {
        if (static_cast<unsigned int>(entry[6]) >= 0x10)
        {
            result = entry[1];
            if (result)
                result = Mem_FreeDebug(reinterpret_cast<void*>(entry[1]));
        }
        entry[6] = 15;
        entry[5] = 0;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(entry) + 4) = 0;  // byte at +4
    }
    return result;
}

HOOK(0x00775200, LevelIndexTable_Destroy, "auto");
