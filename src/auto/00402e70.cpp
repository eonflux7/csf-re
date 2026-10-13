#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Collision_TestTriangle, 0x004026E0, (float* arg0, float* a3, void* arg8, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static uint32_t* __cdecl Collision_TransformAndTestTriangle(
    int /*unused*/, uint32_t* triangle, int /*unused*/, int contextAddress)
{
    alignas(8) uint8_t scaledBytes[0x2C] = {};
    alignas(8) uint8_t transformedBytes[0x24] = {};
    float* scaled = reinterpret_cast<float*>(scaledBytes);
    float* transformed = reinterpret_cast<float*>(transformedBytes);
    void* context = reinterpret_cast<void*>(contextAddress);

    float* vertex0 = reinterpret_cast<float*>(
        CSF_AT(uint32_t, triangle, 0x1C));
    transformed[0] = CSF_AT(float, vertex0, 0x00);

    float* vertex1 = reinterpret_cast<float*>(
        CSF_AT(uint32_t, triangle, 0x20));
    transformed[1] = CSF_AT(float, vertex0, 0x04);
    transformed[2] = CSF_AT(float, vertex0, 0x08);
    transformed[3] = CSF_AT(float, vertex1, 0x00);

    float* vertex2 = reinterpret_cast<float*>(
        CSF_AT(uint32_t, triangle, 0x24));
    transformed[4] = CSF_AT(float, vertex1, 0x04);
    transformed[5] = CSF_AT(float, vertex1, 0x08);
    transformed[6] = CSF_AT(float, vertex2, 0x00);

    int matrixAddress = CSF_AT(int, context, 0x28);
    transformed[7] = CSF_AT(float, vertex2, 0x04);
    transformed[8] = CSF_AT(float, vertex2, 0x08);

    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(transformed),
        3, matrixAddress);

    float inverseX = g_TransitionOne / CSF_AT(float, context, 0x1C);
    float inverseY = g_TransitionOne / CSF_AT(float, context, 0x20);
    float inverseZ = g_TransitionOne / CSF_AT(float, context, 0x24);

    scaled[0] = transformed[0] * inverseX;
    scaled[1] = transformed[1] * inverseY;

    int collisionArgument6 = CSF_AT(int, context, 0x00);
    int collisionArgument5 = CSF_AT(int, context, 0x2C);

    scaled[2] = transformed[2] * inverseZ;
    scaled[3] = transformed[3] * inverseX;
    scaled[4] = transformed[4] * inverseY;
    scaled[5] = transformed[5] * inverseZ;
    scaled[6] = transformed[6] * inverseX;
    scaled[7] = transformed[7] * inverseY;
    scaled[8] = transformed[8] * inverseZ;

    return Collision_TestTriangle(
        scaled,
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(context) + 0x04),
        reinterpret_cast<uint8_t*>(context) + 0x10,
        2, collisionArgument5, collisionArgument6) != 0
        ? triangle
        : nullptr;
}

HOOK(0x00402E70, Collision_TransformAndTestTriangle, "auto");
