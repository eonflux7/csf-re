#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Bounds_GetExtents, 0x006AB4A0, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5, uint32_t* a6, uint32_t* a7));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetBoundsAndDefaultAxes, 0x006ABAA0, (char* self, void* , float maxX, float minX, float maxY, float minY, float maxZ, float minZ));
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(bool, __fastcall, GfxMatrix_RayIntersectsSphere, 0x006ACA40, (float* self, void* , float* sphere));
CSF_FUNC(int, __fastcall, S_00402350_IntersectOrientedBox, 0x006ACB20, (S_00402350* self, void* , const void* box, float* firstDistance, float* secondDistance));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static bool __fastcall C_008637d8_TestRayAgainstBounds(C_008637d8* self, void* /*edx*/, S_00402350* a2, float* a3, float a4)
{
    alignas(8) uint8_t matrixBytes[0x3C];
    alignas(8) uint8_t sphereBytes[0x1C];
    float ext[6];
    float firstDistance;

    float* matrix = reinterpret_cast<float*>(matrixBytes);
    float* sphere = reinterpret_cast<float*>(sphereBytes);

    Bounds_GetExtents(reinterpret_cast<uint8_t*>(self) + 8, nullptr,
                      reinterpret_cast<uint32_t*>(&ext[0]), reinterpret_cast<uint32_t*>(&ext[1]),
                      reinterpret_cast<uint32_t*>(&ext[2]), reinterpret_cast<uint32_t*>(&ext[3]),
                      reinterpret_cast<uint32_t*>(&ext[4]), reinterpret_cast<uint32_t*>(&ext[5]));

    GfxMatrix_SetBoundsAndDefaultAxes(reinterpret_cast<char*>(matrixBytes), nullptr,
                                      ext[0], ext[1], ext[2], ext[3], ext[4], ext[5]);

    GfxMatrix_TransformBasisAndPosition(reinterpret_cast<uint32_t*>(matrixBytes), nullptr,
                                        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x20));

    float scale = a4 + g_TransitionOne;

    CSF_AT(uint32_t, sphereBytes, 0x00) = 0;
    CSF_AT(uint32_t, sphereBytes, 0x04) = 0;
    CSF_AT(uint32_t, sphereBytes, 0x08) = 0;
    CSF_AT(uint32_t, sphereBytes, 0x0C) = 0;

    float v13 = matrix[3] * scale;
    sphere[4] = v13;
    matrix[3] = v13;
    float v14 = matrix[4] * scale;
    sphere[5] = v14;
    matrix[4] = v14;
    float v15 = matrix[5] * scale;
    sphere[6] = v15;
    matrix[5] = v15;

    GfxMatrix_GetBoundingSphere(matrix, nullptr, sphere);

    if (!GfxMatrix_RayIntersectsSphere(reinterpret_cast<float*>(a2), nullptr, sphere))
        return false;

    int r = S_00402350_IntersectOrientedBox(a2, nullptr, matrix, a3, &firstDistance);
    if (r == 1)
        *a3 = 0.0f;
    return r != 0;
}

HOOK(0x0070C410, C_008637d8_TestRayAgainstBounds, "auto");
