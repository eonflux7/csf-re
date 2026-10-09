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
CSF_FUNC(int, __cdecl, HudWidget_MemFreeDebug, 0x690E60, (void* block));

// Resets a range of 48-byte HUD widget records [first, last).
// Each record: +0x04 owned buffer pointer (low byte cleared after release),
// +0x14 length/counter = 0, +0x18 capacity = 15. If the capacity was >= 0x10
// and the buffer pointer was non-null, the buffer is freed.
// The two trailing arguments are part of the original cdecl signature and are unused.
// The return value mirrors the last eax value produced by the loop.
static int __cdecl Hud_ResetWidgetPairRange(int* first, int* last, int unusedA3, int unusedA4)
{
    (void)unusedA3;
    (void)unusedA4;

    int result = 0;
    for (int* widget = first; widget != last; widget += 12)
    {
        if (static_cast<unsigned int>(widget[6]) >= 0x10)
        {
            result = widget[1];
            if (result)
                result = HudWidget_MemFreeDebug(reinterpret_cast<void*>(widget[1]));
        }
        widget[6] = 15;
        widget[5] = 0;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(widget) + 4) = 0;  // byte at +4
    }
    return result;
}

HOOK(0x00433B90, Hud_ResetWidgetPairRange, "auto");
