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

// Resets a range of 60-byte particle emitter buffer records [first, last).
// Each record: +0x04 owned buffer pointer (low byte cleared after release),
// +0x14 length/counter = 0, +0x18 capacity = 15. If the capacity was >= 0x10
// and the buffer pointer was non-null, the buffer is freed.
// The return value mirrors the last eax value produced by the loop (0 if the
// range is empty, since the original's eax is undefined in that case).
static int __cdecl RpPrtStd_ResetParticleBufferRange(int* first, int* last)
{
    int result = 0;
    for (int* record = first; record != last; record += 15)
    {
        if (static_cast<unsigned int>(CSF_AT(int, record, 0x18)) >= 0x10)
        {
            result = CSF_AT(int, record, 0x04);
            if (result)
                result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(int, record, 0x04)));
        }
        CSF_AT(int, record, 0x18) = 15;
        CSF_AT(int, record, 0x14) = 0;
        CSF_AT(uint8_t, record, 0x04) = 0;
    }
    return result;
}

HOOK(0x006E1710, RpPrtStd_ResetParticleBufferRange, "auto");
