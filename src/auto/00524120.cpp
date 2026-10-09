#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00841088_InitInstance, 0x00524080, (void* self, void* edx));
CSF_GLOBAL(int32_t, g_Value_8B0674, 0x008B0674);

// Lazily allocates and initializes the singleton C_00841088 object (124 bytes).
// Returns the cached instance, or null if allocation failed.
// The class is not declared in auto_classes.h, so the instance is handled as void*.
static void* __cdecl C_00841088_GetSingleton()
{
    void* result = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8B0674));
    if (!result)
    {
        result = Mem_Alloc(124);
        if (result)
        {
            result = reinterpret_cast<void*>(C_00841088_InitInstance(result, nullptr));
            g_Value_8B0674 = static_cast<int32_t>(reinterpret_cast<uintptr_t>(result));
        }
        else
        {
            g_Value_8B0674 = 0;
        }
    }
    return result;
}

HOOK(0x524120, C_00841088_GetSingleton, "auto");
