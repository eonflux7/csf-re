#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, FxParticleManager_Construct, 0x0051FD70, (void* self, void* edx));
CSF_GLOBAL(int32_t, g_Value_8b0670, 0x008B0670);

// Lazily allocates and constructs the singleton particle manager (148 bytes).
// Returns the cached instance, or null if allocation failed.
static C_00840fdc* __cdecl FxParticleManager_GetSingleton()
{
    C_00840fdc* result = reinterpret_cast<C_00840fdc*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8b0670)));
    if (!result)
    {
        result = static_cast<C_00840fdc*>(Mem_Alloc(0x94));
        if (result)
        {
            result = reinterpret_cast<C_00840fdc*>(FxParticleManager_Construct(result, nullptr));
            g_Value_8b0670 = static_cast<int32_t>(reinterpret_cast<uintptr_t>(result));
        }
        else
        {
            g_Value_8b0670 = 0;
        }
    }
    return result;
}

HOOK(0x51FFE0, FxParticleManager_GetSingleton, "auto");
