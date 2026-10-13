#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_GLOBAL(int, g_GlobalValue_8B0578, 0x008B0578);

// Lazily allocates a single-byte block on first use and caches its address
// in a global; later calls return the cached value.
static int __cdecl NetProfile_GetOrCreateGlobalBlock() {
    int result = g_GlobalValue_8B0578;
    if (!g_GlobalValue_8B0578) {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(Mem_Alloc(1)));
        g_GlobalValue_8B0578 = result;
    }
    return result;
}

HOOK(0x005189B0, NetProfile_GetOrCreateGlobalBlock, "auto");
