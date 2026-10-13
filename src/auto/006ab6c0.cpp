#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

// Original: RwBBoxAddPoint(float* bbox, float* point), __cdecl, pops nothing.
CSF_FUNC(float*, __cdecl, RwBBoxAddPoint, 0x0063C9B0, (float* a1, float* a2));

// Expands a copy of the bounding box (min at +0, max at +12) by two points
// (point and point + 12 bytes), then writes the expanded box to out.
// Returns out.
static float* __fastcall Collision_CombineBounds(int* self, void* /*edx*/, float* out, float* point)
{
    float bounds[6];
    std::memcpy(bounds, self, sizeof(bounds));

    RwBBoxAddPoint(bounds, point);
    RwBBoxAddPoint(bounds, point + 3);

    std::memcpy(out, bounds, sizeof(bounds));
    return out;
}

HOOK(0x006AB6C0, Collision_CombineBounds, "auto");
