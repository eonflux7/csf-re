#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(unsigned int*, __fastcall, ParticleEmitters_ConstructInstance, 0x006E26E0, (void* self, void* edx));
CSF_GLOBAL(uint32_t, g_Value_8BB738, 0x008BB738);

// Lazily allocates and constructs the shared 128-byte particle emitter instance.
// Returns the cached instance, or null if allocation failed.
static S_0041e5f0* __cdecl ParticleEmitters_GetSharedInstance()
{
    S_0041e5f0* result = reinterpret_cast<S_0041e5f0*>(g_Value_8BB738);
    if (!result)
    {
        result = static_cast<S_0041e5f0*>(Mem_Alloc(128));
        if (result)
        {
            result = reinterpret_cast<S_0041e5f0*>(
                ParticleEmitters_ConstructInstance(result, nullptr));
            g_Value_8BB738 = reinterpret_cast<uint32_t>(result);
        }
        else
        {
            g_Value_8BB738 = 0;
        }
    }
    return result;
}

HOOK(0x6E2760, ParticleEmitters_GetSharedInstance, "auto");
