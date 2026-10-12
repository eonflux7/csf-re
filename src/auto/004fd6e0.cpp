#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void**, __fastcall, AnimSet_ConstructInstance, 0x004FD4A0, (void* self, void* edx));
CSF_GLOBAL(uint32_t, g_Value_8B0564, 0x008B0564);

// Lazily allocates and constructs the cached anim-set singleton (208 bytes).
// Returns the cached instance, or null if allocation failed.
static char** __cdecl CommandoAnims_GetSingleton()
{
    char** result = reinterpret_cast<char**>(g_Value_8B0564);
    if (!result)
    {
        result = static_cast<char**>(Mem_Alloc(0xD0));
        if (result)
        {
            result = reinterpret_cast<char**>(AnimSet_ConstructInstance(result, nullptr));
            g_Value_8B0564 = reinterpret_cast<uint32_t>(result);
        }
        else
        {
            g_Value_8B0564 = 0;
        }
    }
    return result;
}

HOOK(0x4FD6E0, CommandoAnims_GetSingleton, "auto");
