#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, Hud_ConstructInstance, 0x004355D0, (void* self, void* edx));
CSF_GLOBAL(uint32_t, g_Value_8afc78, 0x008AFC78);

// Lazily allocates and constructs the singleton HUD object (820 bytes).
// Returns the cached instance, or null if allocation failed.
static CHud* __cdecl Hud_GetSingleton()
{
    CHud* result = reinterpret_cast<CHud*>(g_Value_8afc78);
    if (!result)
    {
        result = static_cast<CHud*>(Mem_Alloc(0x334));
        if (result)
        {
            result = reinterpret_cast<CHud*>(Hud_ConstructInstance(result, nullptr));
            g_Value_8afc78 = reinterpret_cast<uint32_t>(result);
        }
        else
        {
            g_Value_8afc78 = 0;
        }
    }
    return result;
}

HOOK(0x436030, Hud_GetSingleton, "auto");
