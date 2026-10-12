#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, BddLoader_Construct, 0x00441440, (void* self, void* edx));
CSF_GLOBAL(int, g_Value8afc7c, 0x008AFC7C);

// Lazily allocates and constructs the singleton BDD loader (64 bytes).
// Returns the cached instance, or null if allocation failed.
static C_00831008* __cdecl GameBddLoader_PrepareResources()
{
    C_00831008* result = reinterpret_cast<C_00831008*>(g_Value8afc7c);
    if (!result)
    {
        result = static_cast<C_00831008*>(Mem_Alloc(64));
        if (result)
        {
            result = reinterpret_cast<C_00831008*>(BddLoader_Construct(result, nullptr));
            g_Value8afc7c = static_cast<int>(reinterpret_cast<uintptr_t>(result));
        }
        else
        {
            g_Value8afc7c = 0;
        }
    }
    return result;
}

HOOK(0x441910, GameBddLoader_PrepareResources, "auto");
