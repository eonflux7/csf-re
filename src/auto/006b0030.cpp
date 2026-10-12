#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall Geometry_IsPolygonStrictlyCounterclockwise(
    int* self, void* /*edx*/, int* polygon)
{
    const int count = CSF_AT(int, polygon, 0x08);
    if (count < 3)
        return 1;

    for (int i = 0; i < count; ++i)
    {
        const float* middle =
            CSF_AT(float*, self, 0x00) +
            2 * CSF_AT(int*, polygon, 0x00)[(i + 1) % count];
        const float* next =
            CSF_AT(float*, self, 0x00) +
            2 * CSF_AT(int*, polygon, 0x00)[(i + 2) % count];
        const float* current =
            CSF_AT(float*, self, 0x00) +
            2 * CSF_AT(int*, polygon, 0x00)[i];

        if (!((next[0] - middle[0]) * (middle[1] - current[1]) -
              (next[1] - middle[1]) * (middle[0] - current[0]) < g_fZero))
        {
            return 0;
        }
    }

    return 1;
}

HOOK(0x006B0030, Geometry_IsPolygonStrictlyCounterclockwise, "auto");
