#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_GLOBAL(int32_t, g_Value_8BB4CC, 0x008BB4CC);

// Returns the lazily created 96-byte shared value block (created on first call).
// Returns nullptr when allocation fails (and caches nullptr in the global).
static int32_t* __cdecl SharedValue_GetOrCreateBlock()
{
    int32_t* result = reinterpret_cast<int32_t*>(static_cast<std::uintptr_t>(g_Value_8BB4CC));
    if (!g_Value_8BB4CC)
    {
        result = static_cast<int32_t*>(Mem_Alloc(96));
        if (result)
        {
            CSF_AT(int32_t, result, 0x40) = 0;
            CSF_AT(int32_t, result, 0x44) = 0;
            CSF_AT(int32_t, result, 0x48) = 0;
            CSF_AT(int32_t, result, 0x4C) = 10;
            CSF_AT(int32_t, result, 0x50) = 0;
            CSF_AT(int32_t, result, 0x54) = 0;
            CSF_AT(int32_t, result, 0x58) = 0;
            CSF_AT(int32_t, result, 0x5C) = 10;
            g_Value_8BB4CC = static_cast<int32_t>(reinterpret_cast<std::uintptr_t>(result));
        }
        else
        {
            g_Value_8BB4CC = 0;
        }
    }
    return result;
}

HOOK(0x6C8BC0, SharedValue_GetOrCreateBlock, "auto");
