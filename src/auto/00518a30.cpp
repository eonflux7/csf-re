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

CSF_GLOBAL(int, g_Value_8B057C, 0x008B057C);

// Frees the cached menu-controls highlight buffer (if any) and clears its pointer.
// Returns the global's previous value when it was zero, otherwise Mem_Free's result.
static int __cdecl MenuControls_RefreshHighlight() {
    int result = g_Value_8B057C;
    if (g_Value_8B057C) {
        result = Mem_Free(reinterpret_cast<void*>(g_Value_8B057C));
        g_Value_8B057C = 0;
    }
    return result;
}

HOOK(0x518A30, MenuControls_RefreshHighlight, "auto");
