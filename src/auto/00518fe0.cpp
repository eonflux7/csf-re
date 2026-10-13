#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_GLOBAL(int, g_GlobalValue_8B0584, 0x008B0584);

// Returns the cached 8-byte NetProfile singleton, creating it on first use.
// Word 0 receives initialValue, word 1 is zeroed. Returns nullptr if allocation fails.
static uint32_t* __cdecl NetProfile_GetOrCreateInstance(int initialValue) {
    uint32_t* result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(g_GlobalValue_8B0584));
    if (!g_GlobalValue_8B0584) {
        result = static_cast<uint32_t*>(Mem_Alloc(8));
        if (result) {
            result[0] = static_cast<uint32_t>(initialValue);
            result[1] = 0;
            g_GlobalValue_8B0584 = static_cast<int>(reinterpret_cast<uintptr_t>(result));
        } else {
            g_GlobalValue_8B0584 = 0;
        }
    }
    return result;
}

HOOK(0x518FE0, NetProfile_GetOrCreateInstance, "auto");
