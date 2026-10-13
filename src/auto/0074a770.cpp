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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

CSF_GLOBAL(int, g_Value_8BC170, 0x008BC170);

// Frees the cached network session buffer (if any) and clears its pointer.
// Returns the global's previous value when it was zero, otherwise CoreMemory_Free's result.
static int __cdecl NetSession_FinalizeShutdown() {
    int result = g_Value_8BC170;
    if (g_Value_8BC170) {
        result = CoreMemory_Free(reinterpret_cast<void*>(g_Value_8BC170));
        g_Value_8BC170 = 0;
    }
    return result;
}

HOOK(0x74A770, NetSession_FinalizeShutdown, "auto");
