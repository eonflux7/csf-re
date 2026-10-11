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

// Zero constant the original returns when either argument is null.
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Virtual getter: __thiscall (ecx = this), returns a pointer to a position vector (or an object whose vtable is then queried).
using GfxSkinGetterFn = void* (__fastcall*)(void* self, void* edx);

static double __cdecl GfxSkin_DistanceBetweenPositions(void* a1, void* a2)
{
    if (!a1 || !a2)
        return g_fZero;

    // a2 -> vtbl[4] (offset 0x10) returns an intermediate object
    void* intermediate = reinterpret_cast<GfxSkinGetterFn>(CSF_AT(void**, a2, 0)[4])(a2, nullptr);

    // intermediate -> vtbl[4] (offset 0x10) returns the reference position
    float* referencePos = reinterpret_cast<float*>(
        reinterpret_cast<GfxSkinGetterFn>(CSF_AT(void**, intermediate, 0)[4])(intermediate, nullptr));

    // a1 -> vtbl[18] (offset 0x48) returns the object's position
    float* objectPos = reinterpret_cast<float*>(
        reinterpret_cast<GfxSkinGetterFn>(CSF_AT(void**, a1, 0)[18])(a1, nullptr));

    float deltaX = objectPos[0] - referencePos[0];
    float deltaY = objectPos[1] - referencePos[1];
    float deltaZ = objectPos[2] - referencePos[2];

    return std::sqrt(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX);
}

HOOK(0x006C5050, GfxSkin_DistanceBetweenPositions, "auto");
