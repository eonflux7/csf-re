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
CSF_FUNC(int, __fastcall, S_00402350_IntersectOrientedBox, 0x006ACB20, (S_00402350* self, void* , const void* box, float* firstDistance, float* secondDistance));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall C_008637d8_ComputeHitPoint(C_008637d8* self, void* /*edx*/, S_00402350* a2, int a3, float a4)
{
    alignas(8) uint8_t matrixBytes[0x54];
    float minMaxX, v23, v24, v25, v26, v27;
    float firstDist, secondDist;
    uint8_t* out = reinterpret_cast<uint8_t*>(a3);

    Bounds_GetExtents((uint8_t*)self + 8,
                      nullptr,
                      reinterpret_cast<uint32_t*>(&minMaxX),
                      reinterpret_cast<uint32_t*>(&v23),
                      reinterpret_cast<uint32_t*>(&v24),
                      reinterpret_cast<uint32_t*>(&v25),
                      reinterpret_cast<uint32_t*>(&v26),
                      reinterpret_cast<uint32_t*>(&v27));

    GfxMatrix_SetBoundsAndDefaultAxes(reinterpret_cast<char*>(matrixBytes), nullptr,
                                      minMaxX, v23, v24, v25, v26, v27);
    GfxMatrix_TransformBasisAndPosition(reinterpret_cast<std::uint32_t*>(matrixBytes), nullptr,
                                        reinterpret_cast<std::uint32_t*>((uint8_t*)self + 0x20));

    float scale = a4 + g_TransitionOne;
    float s0 = CSF_AT(float, matrixBytes, 0x0C) * scale;
    CSF_AT(float, matrixBytes, 0x48) = s0;
    CSF_AT(float, matrixBytes, 0x0C) = s0;
    float s1 = CSF_AT(float, matrixBytes, 0x10) * scale;
    CSF_AT(float, matrixBytes, 0x4C) = s1;
    CSF_AT(float, matrixBytes, 0x10) = s1;
    float s2 = CSF_AT(float, matrixBytes, 0x14) * scale;
    CSF_AT(float, matrixBytes, 0x50) = s2;
    CSF_AT(float, matrixBytes, 0x14) = s2;

    int r = S_00402350_IntersectOrientedBox(a2, nullptr, matrixBytes, &firstDist, &secondDist) - 1;
    if (r == 0) {
        CSF_AT(float, out, 0x00) = CSF_AT(float, a2, 0x0C);
        CSF_AT(float, out, 0x04) = CSF_AT(float, a2, 0x10);
        float z = CSF_AT(float, a2, 0x14);
        CSF_AT(float, out, 0x18) = 0.0f;
        CSF_AT(float, out, 0x10) = 0.0f;
        CSF_AT(float, out, 0x14) = 0.0f;
        CSF_AT(float, out, 0x0C) = 1.0f;
        CSF_AT(float, out, 0x08) = z;
        return 1;
    }
    if (r == 1) {
        float x = firstDist * CSF_AT(float, a2, 0x00);
        float y = firstDist * CSF_AT(float, a2, 0x04);
        float z = firstDist * CSF_AT(float, a2, 0x08);
        CSF_AT(float, matrixBytes, 0x44) = z;
        float px = x + CSF_AT(float, a2, 0x0C);
        CSF_AT(float, matrixBytes, 0x48) = px;
        float py = y + CSF_AT(float, a2, 0x10);
        CSF_AT(float, matrixBytes, 0x4C) = py;
        float pz = z + CSF_AT(float, a2, 0x14);
        CSF_AT(float, out, 0x00) = px;
        CSF_AT(float, out, 0x04) = py;
        CSF_AT(float, matrixBytes, 0x50) = pz;
        CSF_AT(float, out, 0x08) = pz;
        CSF_AT(float, out, 0x10) = 0.0f;
        CSF_AT(float, out, 0x14) = 0.0f;
        CSF_AT(float, out, 0x18) = firstDist;
        CSF_AT(float, out, 0x0C) = 1.0f;
        return 1;
    }
    return 0;
}

HOOK(0x0070C510, C_008637d8_ComputeHitPoint, "auto");
