#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original: int __cdecl RpWorldForAllLights(RpWorld* world, callback, void* data)
CSF_FUNC(int, __cdecl, RpWorldForAllLights, 0x0058CF80, (int world, int (__cdecl* callback)(int*, int), int data));

// Original: int __stdcall sub_6D1650(int a1)
// Visits every light in the world a1 through callback sub_6D0F10, with a null user-data pointer.
static int __stdcall GfxLights_ForAllWorldLights(int world) {
    using LightVisitor = int(__cdecl*)(int*, int);
    LightVisitor visitor = reinterpret_cast<LightVisitor>(CSF_ADDR_FN(0x006D0F10));
    return RpWorldForAllLights(world, visitor, 0);
}

HOOK(0x006D1650, GfxLights_ForAllWorldLights, "auto");
