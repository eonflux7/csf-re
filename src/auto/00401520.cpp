#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0082d46c_Construct, 0x004013F0, (void* self, void* edx));
CSF_GLOBAL(int, g_Value_8afc60, 0x008AFC60);

// Lazily allocates and constructs the singleton C_0082d46c object (56 bytes).
// Returns the cached instance, or null if allocation failed.
static C_0082d46c* __cdecl C_0082d46c_GetSingleton()
{
    C_0082d46c* result = reinterpret_cast<C_0082d46c*>(g_Value_8afc60);
    if (!result)
    {
        result = reinterpret_cast<C_0082d46c*>(CoreMemory_AllocUntagged(56));
        if (result)
        {
            result = reinterpret_cast<C_0082d46c*>(C_0082d46c_Construct(result, nullptr));
            g_Value_8afc60 = reinterpret_cast<int>(result);
        }
        else
        {
            g_Value_8afc60 = 0;
        }
    }
    return result;
}

HOOK(0x401520, C_0082d46c_GetSingleton, "auto");
