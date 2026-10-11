#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, Skin_ConstructSingleton, 0x006C72D0, (void* self, void* edx));
CSF_GLOBAL(int, g_Value_8BB4B8, 0x008BB4B8);

// Lazily allocates and constructs the singleton skin object (328 bytes).
// Returns the cached instance, or null if allocation failed.
static C_008608e8* __cdecl Skin_GetSingleton()
{
    C_008608e8* result = reinterpret_cast<C_008608e8*>(g_Value_8BB4B8);
    if (!result)
    {
        result = static_cast<C_008608e8*>(Mem_Alloc(0x148));
        if (result)
        {
            result = reinterpret_cast<C_008608e8*>(Skin_ConstructSingleton(result, nullptr));
            g_Value_8BB4B8 = static_cast<int>(reinterpret_cast<intptr_t>(result));
        }
        else
        {
            g_Value_8BB4B8 = 0;
        }
    }
    return result;
}

HOOK(0x6C74C0, Skin_GetSingleton, "auto");
