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
CSF_FUNC(int, __cdecl, MenuSaveGame_MemFreeDebug, 0x690E60, (void* block));

// Resets a range of 36-byte save-slot records [first, last).
// Each record: +0x04 owned buffer pointer (low byte cleared after release),
// +0x14 length/counter = 0, +0x18 capacity = 15. If the capacity was >= 0x10
// and the buffer pointer was non-null, the buffer is freed.
// The return value mirrors the last eax value produced by the loop.
static int __cdecl MenuSaveGame_ClearSlotRange(int* first, int* last)
{
    int result = 0;
    for (int* slot = first; slot != last; slot += 9)
    {
        if (static_cast<unsigned int>(slot[6]) >= 0x10)
        {
            result = slot[1];
            if (result)
                result = MenuSaveGame_MemFreeDebug(reinterpret_cast<void*>(slot[1]));
        }
        slot[6] = 15;
        slot[5] = 0;
        *reinterpret_cast<uint8_t*>(slot) = 0;  // byte at +4 is slot[1]'s low byte
    }
    return result;
}

HOOK(0x00544340, MenuSaveGame_ClearSlotRange, "auto");
