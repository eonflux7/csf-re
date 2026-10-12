#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// RpWorld / RpLight helpers (cdecl, bound by address)
CSF_FUNC(int, __cdecl, RpWorldAddLight, 0x0058DF50, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpWorldRemoveLight, 0x0058DFA0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Moves the light (provider object) into newWorld, detaching it from its current world first.
// Used by the pseudocode for both light slots at +0x30 and +0x2C.
static int __fastcall ClumpManager_SelectWorld(void* self, void* /*edx*/, int newWorld)
{
    // First light slot (+0x30) - remember the new world at +0xA0.
    C_0085e7b8* firstProviderOwner = *reinterpret_cast<C_0085e7b8**>(CSF_AT(void*, self, 0x30));
    CSF_AT(int, self, 0xA0) = newWorld;

    int firstLight = reinterpret_cast<int>(GetCachedObjectProvider(firstProviderOwner, nullptr));
    if (firstLight != 0)
    {
        int currentWorld = RpLightGetWorld(firstLight);
        if (currentWorld != newWorld)
        {
            if (currentWorld != 0)
                RpWorldRemoveLight(currentWorld, firstLight);
            RpWorldAddLight(newWorld, firstLight);
        }
    }

    // Second light slot (+0x2C).
    C_0085e7b8* secondProviderOwner = *reinterpret_cast<C_0085e7b8**>(CSF_AT(void*, self, 0x2C));
    int secondLight = reinterpret_cast<int>(GetCachedObjectProvider(secondProviderOwner, nullptr));
    int result = secondLight;
    if (secondLight != 0)
    {
        result = RpLightGetWorld(secondLight);
        if (result != newWorld)
        {
            if (result != 0)
                RpWorldRemoveLight(result, secondLight);
            return RpWorldAddLight(newWorld, secondLight);
        }
    }
    return result;
}

HOOK(0x006D0FB0, ClumpManager_SelectWorld, "auto");
