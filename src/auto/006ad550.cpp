#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetBasisAndPosition, 0x006ADA10, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (void* self, void* edx, void** a2));

static float GfxMatrix_ClampToSymmetricExtent(float value, float extent)
{
    float lower = -extent;
    float upper = extent;
    if (lower > upper) {
        lower = extent;
        upper = -extent;
    }

    if (value < lower)
        return lower;
    if (value > upper)
        return upper;
    return value;
}

static float* __cdecl GfxMatrix_ClosestPointOnOrientedBox(
    float* result, int point, float* box)
{
    alignas(4) std::uint8_t forwardHandle[0x8];
    alignas(4) std::uint8_t inverseHandle[0x8];

    CSF_AT(std::uint32_t*, forwardHandle, 0x0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, forwardHandle, 0x4) = 1;

    // The packet labels these direct call targets as vtable references.
    // Neither address is stored into either handle.
    (void)CSF_VTBL(0x006ADA10);
    GfxMatrix_SetBasisAndPosition(
        forwardHandle, nullptr,
        reinterpret_cast<std::uint32_t*>(box + 6),
        reinterpret_cast<std::uint32_t*>(box + 9),
        reinterpret_cast<std::uint32_t*>(box + 12),
        reinterpret_cast<std::uint32_t*>(box));

    CSF_AT(std::uint32_t*, inverseHandle, 0x0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, inverseHandle, 0x4) = 1;

    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        forwardHandle, nullptr, reinterpret_cast<void**>(inverseHandle));

    float transformed[3] = {0.0f, 0.0f, 0.0f};
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed), point, 1,
        reinterpret_cast<int>(CSF_AT(std::uint32_t*, inverseHandle, 0x0)));

    float closest[3] = {transformed[0], transformed[1], transformed[2]};
    float extents[3] = {box[3], box[4], box[5]};

    closest[0] = GfxMatrix_ClampToSymmetricExtent(closest[0], extents[0]);
    closest[1] = GfxMatrix_ClampToSymmetricExtent(closest[1], extents[1]);
    closest[2] = GfxMatrix_ClampToSymmetricExtent(closest[2], extents[2]);

    transformed[0] = 0.0f;
    transformed[1] = 0.0f;
    transformed[2] = 0.0f;
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed), reinterpret_cast<int>(closest), 1,
        reinterpret_cast<int>(CSF_AT(std::uint32_t*, forwardHandle, 0x0)));

    result[0] = transformed[0];
    result[1] = transformed[1];
    result[2] = transformed[2];

    if (CSF_AT(std::uint8_t, inverseHandle, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseHandle, 0x0));
    if (CSF_AT(std::uint8_t, forwardHandle, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, forwardHandle, 0x0));

    return result;
}

HOOK(0x006AD550, GfxMatrix_ClosestPointOnOrientedBox, "auto");
