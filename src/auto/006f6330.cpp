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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Polygon vertex storage: field_0000 points to float pairs (x, y), stride 8 bytes.
// field_0008 is the vertex count.
// The point argument is read as x at +0 and y at +8 (both dwords, as the original does).
static bool __fastcall GfxCamera_IsPointInsideConvexPolygon(S_006f6330* self, void* /*edx*/, int a2)
{
    const int32_t vertexCount = static_cast<int32_t>(self->field_0008);
    int32_t currentIndex = 0;

    if (vertexCount <= 0)
        return true;

    const float* vertices = reinterpret_cast<const float*>(self->field_0000);
    const float pointX = CSF_AT(float, reinterpret_cast<void*>(a2), 0x0);
    const float pointY = CSF_AT(float, reinterpret_cast<void*>(a2), 0x8);

    while (true)
    {
        const int32_t nextIndex = currentIndex + 1;
        const int32_t wrappedIndex = nextIndex % vertexCount;

        const float edgeDx = vertices[wrappedIndex * 2] - vertices[currentIndex * 2];
        const float edgeDy = vertices[wrappedIndex * 2 + 1] - vertices[currentIndex * 2 + 1];
        const float offsetX = pointX - vertices[currentIndex * 2];
        const float offsetY = pointY - vertices[currentIndex * 2 + 1];

        const float side = offsetX * edgeDy - edgeDx * offsetY;
        if (side > g_fZero)
            return false;

        currentIndex = nextIndex;
        if (currentIndex >= vertexCount)
            return true;
    }
}

HOOK(0x6F6330, GfxCamera_IsPointInsideConvexPolygon, "auto");
