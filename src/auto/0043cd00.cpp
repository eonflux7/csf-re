#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

// Computes the radar scale for a size (a2[0], a2[1]) relative to the visible area
// (difference between two world points), clamped by a maximum radius.
static float* __cdecl MinimapRadar_ComputeScaledSize(float* out, int* size)
{
    int base = GetValue8B0554();
    float maxX = CSF_AT(float, base, 0xDB4);
    float maxZ = CSF_AT(float, base, 0xDB4 + 8);

    base = GetValue8B0554();
    float minX = CSF_AT(float, base, 0xDC0);
    float minZ = CSF_AT(float, base, 0xDC0 + 8);

    float extentX = maxX - minX;
    float extentZ = maxZ - minZ;

    float radius = CSF_AT(float, GetValue8B0554(), 0x974);

    double limitX = extentX * g_HalfDurationScale;
    if (radius <= limitX)
        limitX = radius;

    double limitZ = extentZ * g_HalfDurationScale;
    if (radius <= limitZ)
        limitZ = radius;

    double scaleY = limitZ * ((double)size[1] / extentZ);
    out[0] = (float)((double)size[0] / extentX * limitX);
    out[1] = (float)scaleY;
    return out;
}

HOOK(0x0043CD00, MinimapRadar_ComputeScaledSize, "auto");
