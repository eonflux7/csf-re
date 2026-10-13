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

// Resets a range of 0x15C-byte records [first, last) (87 dwords each).
// Each record: +0x04 owned buffer pointer (freed when capacity >= 0x10 and the
// pointer is non-null; the pointer is not cleared afterwards, only the low byte of
// the record at +0x04 is zeroed), +0x14 counter = 0, +0x18 capacity = 15.
// The two trailing arguments are part of the original cdecl signature and are unused.
// The return value mirrors the last value the loop left in result.
static int __cdecl ResetRecordRangeFreeBuffers(int* first, int* last, int unusedA3, int unusedA4)
{
    (void)unusedA3;
    (void)unusedA4;

    int result = 0;
    for (int* record = first; record != last; record += 87)
    {
        if (static_cast<unsigned int>(record[6]) >= 0x10)
        {
            result = record[1];
            if (result)
                result = Mem_FreeDebug(reinterpret_cast<void*>(record[1]));
        }
        record[6] = 15;
        record[5] = 0;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(record) + 4) = 0;  // byte at +4
    }
    return result;
}

HOOK(0x00782A70, ResetRecordRangeFreeBuffers, "auto");
