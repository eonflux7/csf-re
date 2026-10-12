#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up the object's cached provider. Original: __fastcall-style, this in ecx.
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
// RpLight world accessors / world list manipulation (cdecl, RenderWare).
CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveLight, 0x0058DFA0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpWorldAddLight, 0x0058DF50, (int a1, int a2));

// Moves an RpLight (obtained from a1's cached provider) into the world a2.
// If the light already belongs to a2 nothing changes. Otherwise it is removed
// from its current world (if any) and added to a2. Returns the light's
// previous world when no move was needed, otherwise the RpWorldAddLight result.
static int __cdecl GfxLights_MoveLightToWorld(C_0085e7b8* a1, int a2)
{
    int result = static_cast<int>(reinterpret_cast<intptr_t>(
        GetCachedObjectProvider(a1, nullptr)));
    int light = result;
    if (light)
    {
        result = RpLightGetWorld(light);
        if (result != a2)
        {
            if (result)
                RpWorldRemoveLight(result, light);
            return RpWorldAddLight(a2, light);
        }
    }
    return result;
}

HOOK(0x006D0E70, GfxLights_MoveLightToWorld, "auto");
