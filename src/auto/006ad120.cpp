#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool __cdecl GfxMatrix_IsPointInsideTriangle(
    float* point, float* vertexB, float* vertexC, float* vertexD)
{
    const float firstCross =
        (point[0] - vertexB[0]) * (vertexC[1] - vertexB[1]) -
        (point[1] - vertexB[1]) * (vertexC[0] - vertexB[0]);

    const float secondCross =
        (point[0] - vertexC[0]) * (vertexD[1] - vertexC[1]) -
        (point[1] - vertexC[1]) * (vertexD[0] - vertexC[0]);

    // The original comparison also accepts unordered (NaN) results.
    if (secondCross * firstCross < g_fZero)
        return false;

    const float thirdCross =
        (point[0] - vertexD[0]) * (vertexB[1] - vertexD[1]) -
        (point[1] - vertexD[1]) * (vertexB[0] - vertexD[0]);

    return !(thirdCross * firstCross < g_fZero);
}

HOOK(0x006AD120, GfxMatrix_IsPointInsideTriangle, "auto");
