#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns 1 when the point a2 lies on the non-negative side of all five
// culling planes stored in the object (four floats per plane: nx, ny, nz, d),
// after offsetting by a3. Returns 0 if the object's enable byte is clear or any
// plane test fails.
static char __fastcall GfxScene_IsPointInsideCullPlanes(float* self, void* /*edx*/, float* a2, float a3)
{
    if (!CSF_AT(uint8_t, self, 0x6A))
        return 0;

    // First plane's normal component starts at self+0x18; the loop body
    // reads the four floats of each plane (nx, ny, nz, d).
    float* plane = self + 7;
    for (int planeIndex = 0; planeIndex < 5; ++planeIndex, plane += 4)
    {
        float planeDot = plane[1] * a2[2]
                       + plane[-1] * a2[0]
                       + a2[1] * plane[0]
                       - plane[2]
                       - a3;
        if (planeDot < g_fZero)
            return 0;
    }
    return 1;
}

HOOK(0x6CA870, GfxScene_IsPointInsideCullPlanes, "auto");
