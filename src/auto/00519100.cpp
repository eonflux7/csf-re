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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(int, g_GlobalValue_8B058C, 0x008B058C);

// Frees the cached NetProfile buffer (if any) and clears its pointer.
// Returns the global's previous value when it was zero, otherwise Mem_Free's result.
static int __cdecl NetProfile_FreeTooltipBuffer() {
    int result = g_GlobalValue_8B058C;
    if (g_GlobalValue_8B058C) {
        result = Mem_Free(reinterpret_cast<void*>(g_GlobalValue_8B058C));
        g_GlobalValue_8B058C = 0;
    }
    return result;
}

HOOK(0x519100, NetProfile_FreeTooltipBuffer, "auto");
