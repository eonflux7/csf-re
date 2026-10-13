#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_GLOBAL(int, g_Value_8B057C, 0x008B057C);

// Lazily allocates a single-byte block on first use and caches its address
// in a global; later calls return the cached value.
static int __cdecl NetProfile_GetOrCreateCachedBlock() {
    int result = g_Value_8B057C;
    if (!g_Value_8B057C) {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(Mem_Alloc(1)));
        g_Value_8B057C = result;
    }
    return result;
}

HOOK(0x00518A00, NetProfile_GetOrCreateCachedBlock, "auto");
