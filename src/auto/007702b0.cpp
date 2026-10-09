#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_76A540, 0x0076A540, (void* self, void* edx));
CSF_GLOBAL(uint32_t, g_Value_8BC1E8, 0x008BC1E8);

// Lazily allocates and constructs the singleton input-action binding table (16 bytes).
// Returns the cached instance, or null if allocation failed.
static void* __cdecl InputBindings_GetSingleton()
{
    void* result = reinterpret_cast<void*>(g_Value_8BC1E8);
    if (!result)
    {
        result = Mem_Alloc(16);
        if (result)
        {
            result = reinterpret_cast<void*>(sub_76A540(result, nullptr));
            g_Value_8BC1E8 = reinterpret_cast<uint32_t>(result);
        }
        else
        {
            g_Value_8BC1E8 = 0;
        }
    }
    return result;
}

HOOK(0x7702B0, InputBindings_GetSingleton, "auto");
