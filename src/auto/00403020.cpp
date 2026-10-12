#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00403020
// Builds the 8 corners of a box, brings them into the query's scaled space (divides by the
// query scale at +0x1C/+0x20/+0x24) and feeds the 12 triangles of the box, one by one, to the
// triangle test. Stops at the first triangle for which the test returns 0.

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(char, __cdecl, Collision_TestTriangle, 0x004026E0, (float* arg0, float* a3, void* arg8, int a4, int a5, int a6));
CSF_FUNC(float*, __fastcall, S_006abb80_WriteBoxCorners, 0x006ABC40, (S_006abb80* self, void* , float* corners));

// Corner indices (into the 8 corners written by WriteBoxCorners) of the 12 box triangles,
// in the order the original tests them.
static const uint8_t s_BoxTriangleCorners[12][3] = {
    { 2, 1, 0 },
    { 0, 3, 2 },
    { 6, 7, 4 },
    { 6, 4, 5 },
    { 1, 2, 6 },
    { 6, 5, 1 },
    { 7, 3, 0 },
    { 4, 7, 0 },
    { 3, 7, 6 },
    { 6, 2, 3 },
    { 5, 4, 0 },
    { 0, 1, 5 },
};

static char __cdecl Collision_TestScaledBoxTriangles(S_006abb80* box, void* query)
{
    alignas(8) float triangle[0x2C / sizeof(float)] = {};   // 3 vertices (9 floats) + frame slack
    alignas(8) float corners[0x60 / sizeof(float)] = {};    // 8 corners * xyz

    S_006abb80_WriteBoxCorners(box, nullptr, corners);

    const float invScaleX = g_TransitionOne / CSF_AT(float, query, 0x1C);
    const float invScaleY = g_TransitionOne / CSF_AT(float, query, 0x20);
    const float invScaleZ = g_TransitionOne / CSF_AT(float, query, 0x24);

    for (int corner = 0; corner < 8; ++corner)
    {
        corners[corner * 3 + 0] = corners[corner * 3 + 0] * invScaleX;
        corners[corner * 3 + 1] = corners[corner * 3 + 1] * invScaleY;
        corners[corner * 3 + 2] = corners[corner * 3 + 2] * invScaleZ;
    }

    char result = 0;
    for (int tri = 0; tri < 12; ++tri)
    {
        for (int vertex = 0; vertex < 3; ++vertex)
        {
            const float* src = &corners[s_BoxTriangleCorners[tri][vertex] * 3];
            triangle[vertex * 3 + 0] = src[0];
            triangle[vertex * 3 + 1] = src[1];
            triangle[vertex * 3 + 2] = src[2];
        }

        result = Collision_TestTriangle(triangle,
                                        &CSF_AT(float, query, 0x4),
                                        (uint8_t*)query + 0x10,
                                        2,
                                        CSF_AT(int, query, 0x2C),
                                        CSF_AT(int, query, 0x0));
        if (!result)
            return result;
    }
    return result;
}

HOOK(0x00403020, Collision_TestScaledBoxTriangles, "auto");
