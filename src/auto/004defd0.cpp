#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __cdecl Geometry_IntersectSegments(
    float firstX, float firstY,
    float firstDirectionX, float firstDirectionY,
    float secondX, float secondY,
    float secondDirectionX, float secondDirectionY,
    float* firstParameter, float* secondParameter)
{
    const float determinant =
        firstDirectionY * secondDirectionX -
        firstDirectionX * secondDirectionY;

    const float deltaX = secondX - firstX;
    const float deltaY = secondY - firstY;
    const float firstNumerator =
        secondDirectionX * deltaY - deltaX * secondDirectionY;
    const float secondNumerator =
        deltaY * firstDirectionX - deltaX * firstDirectionY;

    const bool intersects =
        (determinant > g_fZero &&
         firstNumerator >= g_fZero &&
         secondNumerator >= g_fZero &&
         firstNumerator < determinant &&
         secondNumerator < determinant) ||
        (determinant < g_fZero &&
         firstNumerator <= g_fZero &&
         secondNumerator <= g_fZero &&
         firstNumerator > determinant &&
         secondNumerator > determinant);

    if (!intersects)
        return 0;

    const float reciprocal = g_TransitionOne / determinant;
    *firstParameter = firstNumerator * reciprocal;
    *secondParameter = reciprocal * secondNumerator;
    return 1;
}

HOOK(0x004DEFD0, Geometry_IntersectSegments, "auto");
