#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, HudStringShow_ConstructInstance, 0x004F4BD0, (void* self, void* edx));
CSF_GLOBAL(int32_t, g_Value8B0558, 0x008B0558);

static uint32_t* __cdecl HudStringShow_GetSingleton()
{
    uint32_t* result = reinterpret_cast<uint32_t*>(g_Value8B0558);
    if (!result)
    {
        result = static_cast<uint32_t*>(Mem_Alloc(16));
        if (result)
        {
            result = HudStringShow_ConstructInstance(result, nullptr);
            g_Value8B0558 = reinterpret_cast<int32_t>(result);
        }
        else
        {
            g_Value8B0558 = 0;
        }
    }
    return result;
}

HOOK(0x004F4D20, HudStringShow_GetSingleton, "auto");
