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

CSF_GLOBAL(int, g_GlobalValue_8BB404, 0x008BB404);
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

// Runs one filesystem dispatch call with the default filesystem bound to the
// context object at a1+0x38, then restores the previously bound one from g_008bb400.
static int __cdecl RtFs_InvokeWithDefaultFileSystem(int a1, int a2, int a3)
{
    void* context = reinterpret_cast<void*>(a1);
    int result;

    CSF_AT(int, context, 0x38) = g_GlobalValue_8BB404;

    // Dispatch entry lives at +0x30 of the table pointed to by g_GlobalValue_8BB404.
    uint32_t dispatchTable = static_cast<uint32_t>(g_GlobalValue_8BB404);
    auto dispatch = reinterpret_cast<int(__cdecl*)(int, int, int)>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(dispatchTable), 0x30));
    result = dispatch(a1, a2, a3);

    CSF_AT(int, context, 0x38) = g_008bb400;
    return result;
}

HOOK(0x0069FA50, RtFs_InvokeWithDefaultFileSystem, "auto");
