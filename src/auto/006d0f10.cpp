#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RpLightGetWorld, 0x0058DFD0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveLight, 0x0058DFA0, (int a1, int a2));

// Removes an RpLight from the world it belongs to (if any) and returns the light pointer unchanged.
static int __cdecl GfxLights_RemoveLightFromWorld(int light) {
    if (light) {
        int world = RpLightGetWorld(light);
        if (world) {
            RpWorldRemoveLight(world, light);
        }
    }
    return light;
}

HOOK(0x006D0F10, GfxLights_RemoveLightFromWorld, "auto");
