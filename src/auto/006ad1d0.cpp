#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool __cdecl GfxMatrix_IsPointInsideTriangle(
    float* point, float* vertexB, float* vertexC, float* vertexD)
{
    const float pointFromBX = point[0] - vertexB[0];
    const float pointFromBY = point[1] - vertexB[1];
    const float pointFromBZ = point[2] - vertexB[2];
    const float edgeBCX = vertexC[0] - vertexB[0];
    const float edgeBCY = vertexC[1] - vertexB[1];
    const float edgeBCZ = vertexC[2] - vertexB[2];

    const float normalBX = edgeBCY * pointFromBZ - edgeBCZ * pointFromBY;
    const float normalBY = edgeBCZ * pointFromBX - pointFromBZ * edgeBCX;
    const float normalBZ = edgeBCX * pointFromBY - edgeBCY * pointFromBX;

    const float pointFromCX = point[0] - vertexC[0];
    const float pointFromCY = point[1] - vertexC[1];
    const float pointFromCZ = point[2] - vertexC[2];
    const float edgeCDX = vertexD[0] - vertexC[0];
    const float edgeCDY = vertexD[1] - vertexC[1];
    const float edgeCDZ = vertexD[2] - vertexC[2];

    const float normalCX = edgeCDY * pointFromCZ - edgeCDZ * pointFromCY;
    const float normalCY = edgeCDZ * pointFromCX - pointFromCZ * edgeCDX;
    const float normalCZ = pointFromCY * edgeCDX - edgeCDY * pointFromCX;

    if (normalCY * normalBY + normalCX * normalBX + normalCZ * normalBZ < g_fZero)
        return false;

    const float pointFromDX = point[0] - vertexD[0];
    const float pointFromDY = point[1] - vertexD[1];
    const float pointFromDZ = point[2] - vertexD[2];
    const float edgeDBX = vertexB[0] - vertexD[0];
    const float edgeDBY = vertexB[1] - vertexD[1];
    const float edgeDBZ = vertexB[2] - vertexD[2];

    const float normalDX = edgeDBY * pointFromDZ - edgeDBZ * pointFromDY;
    const float normalDY = edgeDBZ * pointFromDX - pointFromDZ * edgeDBX;
    const float normalDZ = pointFromDY * edgeDBX - edgeDBY * pointFromDX;

    // The original accepts unordered comparisons as well as nonnegative results.
    return !(normalDY * normalBY + normalDX * normalBX + normalDZ * normalBZ < g_fZero);
}

HOOK(0x006AD1D0, GfxMatrix_IsPointInsideTriangle, "auto");
