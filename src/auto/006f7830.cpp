#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_Value8BBB28, 0x008BBB28);

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Returns the shared 16-byte scene-services block, creating it on first use.
// Layout: [0] = 0, [1] = 0, [2] = 0, [3] = 10. Returns null (and stores null) if allocation fails.
static int* __cdecl GfxMaterials_GetOrCreateSceneServices()
{
    int cachedValue = g_Value8BBB28;
    int* result = reinterpret_cast<int*>(static_cast<std::uintptr_t>(cachedValue));

    if (cachedValue == 0)
    {
        result = reinterpret_cast<int*>(
            static_cast<std::uintptr_t>(CoreMemory_AllocUntagged(16)));

        if (result != nullptr)
        {
            result[0] = 0;
            result[1] = 0;
            result[2] = 0;
            result[3] = 10;
            g_Value8BBB28 = static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
        }
        else
        {
            g_Value8BBB28 = 0;
        }
    }

    return result;
}

HOOK(0x006F7830, GfxMaterials_GetOrCreateSceneServices, "auto");
