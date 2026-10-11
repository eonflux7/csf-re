#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Bounds_GetExtents, 0x006AB4A0, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5, uint32_t* a6, uint32_t* a7));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetBoundsAndDefaultAxes, 0x006ABAA0, (char* self, void* , float maxX, float minX, float maxY, float minY, float maxZ, float minZ));
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));

// Reads the bounding extents of the sub-object at +0x08, rebuilds the matrix
// from them, applies the transform to the matrix stored at +0x20, and copies
// the 15-dword result into the caller-supplied buffer.
static void* __fastcall C_008637d8_FillBoundsMatrix(C_008637d8* self, void* /*edx*/, void* outMatrix)
{
    alignas(8) uint8_t matrixBuf[0x3C];

    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;

    Bounds_GetExtents(
        reinterpret_cast<uint8_t*>(self) + 0x08, nullptr,
        reinterpret_cast<uint32_t*>(&maxX),
        reinterpret_cast<uint32_t*>(&minX),
        reinterpret_cast<uint32_t*>(&maxY),
        reinterpret_cast<uint32_t*>(&minY),
        reinterpret_cast<uint32_t*>(&maxZ),
        reinterpret_cast<uint32_t*>(&minZ));

    GfxMatrix_SetBoundsAndDefaultAxes(
        reinterpret_cast<char*>(matrixBuf), nullptr,
        maxX, minX, maxY, minY, maxZ, minZ);

    GfxMatrix_TransformBasisAndPosition(
        reinterpret_cast<std::uint32_t*>(matrixBuf), nullptr,
        reinterpret_cast<std::uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x20));

    std::memcpy(outMatrix, matrixBuf, 0x3C);
    return outMatrix;
}

HOOK(0x0070BA50, C_008637d8_FillBoundsMatrix, "auto");
