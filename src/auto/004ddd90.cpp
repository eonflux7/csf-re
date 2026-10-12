#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __cdecl Geometry_ClassifySegmentRectangleIntersection(
    float x, float y, float deltaX, float deltaY,
    float minX, float minY, float maxX, float maxY)
{
    unsigned int startCode = x <= minX;
    if (x >= maxX)
        startCode |= 2u;
    if (y <= minY)
        startCode |= 4u;
    if (y >= maxY)
        startCode |= 8u;

    const float endX = x + deltaX;
    const float endY = y + deltaY;
    unsigned int endCode = endX <= minX;
    if (endX >= maxX)
        endCode |= 2u;
    if (endY <= minY)
        endCode |= 4u;
    if (endY >= maxY)
        endCode |= 8u;

    if ((endCode & startCode) != 0)
        return 0;
    if ((endCode | startCode) == 0)
        return 7;

    const float negativeHeightProduct = -((maxY - minY) * deltaX);
    const float widthProduct = (maxX - minX) * deltaY;
    const float cornerCross = (minX - x) * deltaY - (minY - y) * deltaX;

    if (cornerCross > g_fZero)
    {
        const float heightProduct = -negativeHeightProduct;
        if (cornerCross > heightProduct &&
            -widthProduct < cornerCross &&
            heightProduct - widthProduct < cornerCross)
            return 0;
    }

    if (cornerCross < g_fZero)
    {
        const float heightProduct = -negativeHeightProduct;
        if (cornerCross < heightProduct &&
            -widthProduct > cornerCross &&
            heightProduct - widthProduct > cornerCross)
            return 0;
    }

    int result = 4;
    if (!startCode)
        result = 5;
    if (!endCode)
        return result | 2;
    return result;
}

HOOK(0x004DDD90, Geometry_ClassifySegmentRectangleIntersection, "auto");
