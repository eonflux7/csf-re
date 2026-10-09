#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, SoundManager_ConstructInstance, 0x00525810, (void* self, void* edx));
CSF_GLOBAL(int, g_SoundManager_8b0678, 0x008B0678);

// Lazily allocates and constructs the singleton sound manager (156 bytes).
// Returns the cached instance, or null if allocation failed.
static C_00841090* __cdecl SoundManager_GetSingleton()
{
    C_00841090* result = reinterpret_cast<C_00841090*>(g_SoundManager_8b0678);
    if (!result)
    {
        result = static_cast<C_00841090*>(Mem_Alloc(0x9C));
        if (result)
        {
            result = reinterpret_cast<C_00841090*>(SoundManager_ConstructInstance(result, nullptr));
            g_SoundManager_8b0678 = reinterpret_cast<int>(result);
        }
        else
        {
            g_SoundManager_8b0678 = 0;
        }
    }
    return result;
}

HOOK(0x5258B0, SoundManager_GetSingleton, "auto");
