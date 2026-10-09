#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, MissionSetup_ConstructInstance, 0x004E9EB0, (void* self, void* edx));
CSF_GLOBAL(uint32_t, g_Value8B0554, 0x008B0554);

// Lazily allocates and constructs the singleton mission-setup object (0x2000 bytes).
// Returns the cached instance, or null if allocation failed.
static C_0085e7b8* __cdecl MissionSetup_GetSingleton()
{
    C_0085e7b8* result = reinterpret_cast<C_0085e7b8*>(g_Value8B0554);
    if (!result)
    {
        result = static_cast<C_0085e7b8*>(Mem_Alloc(0x2000));
        if (result)
        {
            result = reinterpret_cast<C_0085e7b8*>(MissionSetup_ConstructInstance(result, nullptr));
            g_Value8B0554 = reinterpret_cast<uint32_t>(result);
        }
        else
        {
            g_Value8B0554 = 0;
        }
    }
    return result;
}

HOOK(0x4EA0D0, MissionSetup_GetSingleton, "auto");
