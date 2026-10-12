#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Computes the min/max float extents of a bounding pair via 0x4E09B0, then
// applies each extent pair to `this` via 0x4DDF80 (the second call's result is returned).
CSF_FUNC(unsigned int, __fastcall, ComputeFloatExtents, 0x004E09B0,
         (void* self, void* edx, float* a2, float* a3, float* a4, float* a5));
CSF_FUNC(char, __fastcall, ApplyExtentToObject, 0x004DDF80,
         (void* self, void* edx, float a2, float a3, uint32_t* a4, uint32_t* a5));

static char __fastcall S_004ddf20_ProcessExtentPair(S_004ddf20* self, void* /*edx*/,
                                                    int a2, uint32_t* a3, uint32_t* a4,
                                                    uint32_t* a5, uint32_t* a6)
{
    // Sentinel starting values: minimum = +1e8, maximum = -1e8.
    float minX = 100000000.0f;
    float minY = 100000000.0f;
    float maxX = -100000000.0f;
    float maxY = -100000000.0f;

    // ecx for the 0x4E09B0 call is loaded from [ebp+8] (the first stack argument, a2)
    // and is not overwritten before the call, so it is passed as `this`.
    void* extentThis = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    ComputeFloatExtents(extentThis, nullptr, &minX, &minY, &maxX, &maxY);

    // Re-read the extents after the callee has filled them.
    ApplyExtentToObject(self, nullptr, minX, minY, a3, a4);
    return ApplyExtentToObject(self, nullptr, maxX, maxY, a5, a6);
}

HOOK(0x004DE230, S_004ddf20_ProcessExtentPair, "auto");
