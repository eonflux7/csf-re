#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int*, __fastcall, DynamicObj_ConstructMapLoad, 0x004428C0, (void* self, void* edx));
CSF_GLOBAL(int, g_Value_8afc80, 0x008AFC80);

// Lazily allocates and constructs the singleton map-load object (32 bytes).
// Returns the cached instance, or null if allocation failed.
static int* __cdecl DynamicObj_GetMapLoadSingleton()
{
    int* result = reinterpret_cast<int*>(g_Value_8afc80);
    if (!result)
    {
        result = static_cast<int*>(Mem_Alloc(0x20));
        if (result)
        {
            result = DynamicObj_ConstructMapLoad(result, nullptr);
            g_Value_8afc80 = reinterpret_cast<int>(result);
        }
        else
        {
            g_Value_8afc80 = 0;
        }
    }
    return result;
}

HOOK(0x4429D0, DynamicObj_GetMapLoadSingleton, "auto");
