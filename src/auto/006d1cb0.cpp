#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldAddLight, 0x0058DF50, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpWorldRemoveLight, 0x0058DFA0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Reconciles an RpLight's world membership with the world stored in this object's
// field_0088 (used when field_0090 is set), or removes the light from its current
// world when field_0090 is clear.
static int __fastcall C_00860e68_ApplyLightWorldAssignment(C_00860e68* self, void* /*edx*/, C_0085e7b8* a2)
{
    int result;

    if (self->field_0090)
    {
        // field_0088 points at an object whose first dword is the target world.
        int targetWorld = *reinterpret_cast<int*>(self->field_0088);
        result = reinterpret_cast<int>(GetCachedObjectProvider(a2, nullptr));
        int light = result;
        if (result)
        {
            result = RpLightGetWorld(light);
            if (result != targetWorld)
            {
                if (result)
                    RpWorldRemoveLight(result, light);
                return RpWorldAddLight(targetWorld, light);
            }
        }
    }
    else
    {
        result = reinterpret_cast<int>(GetCachedObjectProvider(a2, nullptr));
        int light = result;
        if (result)
        {
            result = RpLightGetWorld(light);
            if (result)
                return RpWorldRemoveLight(result, light);
        }
    }
    return result;
}

HOOK(0x006D1CB0, C_00860e68_ApplyLightWorldAssignment, "auto");
