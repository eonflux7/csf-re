#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_GLOBAL(int, g_Value_8B0588, 0x008B0588);

// Returns the cached 8-byte NetProfile instance stored at g_Value_8B0588, creating it on first use.
// Word 0 receives the first argument, word 1 is zeroed. Returns nullptr if allocation fails.
static uint32_t* __cdecl NetProfile_GetOrCreateCachedInstance(int initialValue) {
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(g_Value_8B0588));
    if (!g_Value_8B0588) {
        result = static_cast<uint32_t*>(Mem_Alloc(8));
        if (result) {
            result[0] = static_cast<uint32_t>(initialValue);
            result[1] = 0;
            g_Value_8B0588 = static_cast<int>(reinterpret_cast<uintptr_t>(result));
        } else {
            g_Value_8B0588 = 0;
        }
    }
    return result;
}

HOOK(0x519050, NetProfile_GetOrCreateCachedInstance, "auto");
