#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __cdecl, RtIntersectionSphereTriangle, 0x00620840, (float* a1, float* a2, float* a3, float* a4, float* a5, float* a6));

static int __fastcall Geometry_FindIntersectingQuad(
    void* self, void* /*edx*/, float* ellipsoid)
{
    float radii[3] = {ellipsoid[3], ellipsoid[4], ellipsoid[5]};
    float center[3] = {ellipsoid[0], ellipsoid[1], ellipsoid[2]};
    float firstVertex[3] = {};
    float secondVertex[6] = {};
    float thirdVertex[3] = {};
    float intersection[5] = {};

    const float inverseX = g_TransitionOne / radii[0];
    center[0] *= inverseX;

    const float inverseY = g_TransitionOne / radii[1];
    center[1] *= inverseY;

    const float inverseZ = g_TransitionOne / radii[2];
    const uint32_t initialCount = CSF_AT(uint32_t, self, 0xF0);
    center[2] *= inverseZ;

    float sphere[4] = {center[0], center[1], center[2], 1.0f};
    secondVertex[3] = center[0];
    secondVertex[4] = center[1];
    secondVertex[5] = center[2];

    int result = -1;
    uint32_t index = 0;
    uint32_t byteOffset = 0;

    if (initialCount == 0)
        return result;

    while (result == -1)
    {
        auto* quad = reinterpret_cast<uint8_t*>(
            CSF_AT(float*, self, 0xE8)) + byteOffset;

        firstVertex[0] = inverseX * CSF_AT(float, quad, 0x00);
        firstVertex[1] = inverseY * CSF_AT(float, quad, 0x04);
        firstVertex[2] = inverseZ * CSF_AT(float, quad, 0x08);

        secondVertex[0] = inverseX * CSF_AT(float, quad, 0x0C);
        secondVertex[1] = inverseY * CSF_AT(float, quad, 0x10);
        secondVertex[2] = inverseZ * CSF_AT(float, quad, 0x14);

        thirdVertex[0] = inverseX * CSF_AT(float, quad, 0x18);
        thirdVertex[1] = inverseY * CSF_AT(float, quad, 0x1C);
        thirdVertex[2] = inverseZ * CSF_AT(float, quad, 0x20);

        int intersects = RtIntersectionSphereTriangle(
            sphere, firstVertex, secondVertex, thirdVertex,
            radii, intersection);

        if (!intersects)
        {
            secondVertex[0] = inverseX * CSF_AT(float, quad, 0x24);
            secondVertex[1] = inverseY * CSF_AT(float, quad, 0x28);
            secondVertex[2] = inverseZ * CSF_AT(float, quad, 0x2C);

            intersects = RtIntersectionSphereTriangle(
                sphere, thirdVertex, secondVertex, firstVertex,
                radii, intersection);
        }

        if (intersects)
            result = static_cast<int>(index);

        const uint32_t count = CSF_AT(uint32_t, self, 0xF0);
        ++index;
        byteOffset += 48;
        if (index >= count)
            return result;
    }

    return result;
}

HOOK(0x0044FDD0, Geometry_FindIntersectingQuad, "auto");
