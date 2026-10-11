#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorCopy, 0x0061F120, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetEulerAngles, 0x006AE3A0, (float** self, void* , float* angles));
CSF_FUNC(float*, __cdecl, RtQuatRotate, 0x007D9E30, (int a1, float* a2, float a3, int a4));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);
CSF_GLOBAL(float, g_RadiansToDegrees, 0x00861DF8);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);

static std::uint8_t __fastcall GfxHAnim_AlignAnimationYaw(
    void* self, void* /*edx*/, std::uint32_t* hierarchies,
    std::uint32_t* animation)
{
    alignas(8) std::uint8_t rotationAxisBytes[0xC];
    alignas(8) std::uint8_t eulerAnglesBytes[0xC];
    alignas(8) std::uint8_t referenceMatrixBytes[0x8];
    alignas(8) std::uint8_t nodeMatrixBytes[0x8];
    alignas(8) std::uint8_t inverseMatrixBytes[0x8];
    alignas(8) std::uint8_t rootMatrixBytes[0x10];

    auto& rootMatrix = *reinterpret_cast<S_006adb40*>(rootMatrixBytes);
    auto* angles = reinterpret_cast<float*>(eulerAnglesBytes);

    // Binding required by the packet for the matrix helper.
    (void)CSF_VTBL(0x006ADB40);

    RpHAnimHierarchyUpdateMatrices(reinterpret_cast<int*>(hierarchies[0]));
    rootMatrix.field_0000 =
        RpHAnimHierarchyGetMatrixArray(static_cast<int>(hierarchies[0]));
    rootMatrix.pad_0004[0] = 0;

    CSF_AT(float*, inverseMatrixBytes, 0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(std::uint8_t, inverseMatrixBytes, 4) = 1;
    Matrix_InvertWithStoredTarget(
        &rootMatrix, nullptr, reinterpret_cast<float**>(inverseMatrixBytes));

    CSF_AT(std::uint32_t, eulerAnglesBytes, 0) = 0;
    CSF_AT(std::uint32_t, eulerAnglesBytes, 4) = 0;
    CSF_AT(std::uint32_t, eulerAnglesBytes, 8) = 0;
    RwV3dTransformVectors(
        reinterpret_cast<int>(angles),
        reinterpret_cast<int>(&g_AxisY), 1,
        reinterpret_cast<int>(CSF_AT(float*, inverseMatrixBytes, 0)));

    CSF_AT(std::uint32_t, rotationAxisBytes, 0) =
        CSF_AT(std::uint32_t, eulerAnglesBytes, 0);
    CSF_AT(std::uint32_t, rotationAxisBytes, 4) =
        CSF_AT(std::uint32_t, eulerAnglesBytes, 4);
    CSF_AT(std::uint32_t, rotationAxisBytes, 8) =
        CSF_AT(std::uint32_t, eulerAnglesBytes, 8);

    CSF_AT(float*, referenceMatrixBytes, 0) =
        reinterpret_cast<float*>(RpHAnimHierarchyGetMatrixArray(
            CSF_AT(int, self, 0x14)));
    CSF_AT(std::uint8_t, referenceMatrixBytes, 4) = 0;

    CSF_AT(float, rootMatrixBytes, 0xC) =
        GfxMatrix_GetEulerAngles(
            reinterpret_cast<float**>(referenceMatrixBytes), nullptr, angles)[1]
        * g_RadiansToDegrees;
    if (CSF_AT(float, rootMatrixBytes, 0xC) < g_fZero)
        CSF_AT(float, rootMatrixBytes, 0xC) += g_FullRotationDegrees;

    const std::uint32_t matrixArray = static_cast<std::uint32_t>(
        RpHAnimHierarchyGetMatrixArray(CSF_AT(int, self, 0x14)));
    CSF_AT(float*, nodeMatrixBytes, 0) = reinterpret_cast<float*>(
        matrixArray + (CSF_AT(std::uint32_t, self, 0x30) << 6));
    CSF_AT(std::uint8_t, nodeMatrixBytes, 4) = 0;

    float nodeYaw = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(nodeMatrixBytes), nullptr, angles)[1]
        * g_RadiansToDegrees;
    if (nodeYaw < g_fZero)
        nodeYaw += g_FullRotationDegrees;

    rootMatrix.field_0008 = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(rootMatrixBytes), nullptr, angles)[1]
        * g_RadiansToDegrees;

    RtAnimInterpolatorSetCurrentAnim(
        CSF_AT(std::uint32_t*, reinterpret_cast<void*>(hierarchies[0]), 0x20),
        animation);
    RpHAnimHierarchyUpdateMatrices(reinterpret_cast<int*>(hierarchies[0]));

    const float animatedYaw = GfxMatrix_GetEulerAngles(
        reinterpret_cast<float**>(rootMatrixBytes), nullptr, angles)[1]
        * g_RadiansToDegrees;

    auto* sourceHierarchy = CSF_AT(
        void*, reinterpret_cast<void*>(hierarchies[1]), 0x8);
    auto* sourceInterpolator =
        CSF_AT(std::uint8_t*, sourceHierarchy, 0x20);

    float rotation = nodeYaw - CSF_AT(float, rootMatrixBytes, 0xC);
    if (nodeYaw > CSF_AT(float, rootMatrixBytes, 0xC) &&
        rotation < g_PositiveHalfRotationDegrees)
    {
        rotation -= animatedYaw - rootMatrix.field_0008;
    }

    RtQuatRotate(
        reinterpret_cast<int>(sourceInterpolator + 0x54),
        reinterpret_cast<float*>(rotationAxisBytes), rotation, 1);

    sourceHierarchy = CSF_AT(
        void*, reinterpret_cast<void*>(hierarchies[1]), 0x8);
    const int sourceAnimation = CSF_AT(int, sourceHierarchy, 0x20);
    auto* destinationInterpolator = CSF_AT(
        std::uint32_t*, reinterpret_cast<void*>(hierarchies[0]), 0x20);
    RtAnimInterpolatorCopy(destinationInterpolator, sourceAnimation);

    if (CSF_AT(std::uint8_t, nodeMatrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, nodeMatrixBytes, 0));
    if (CSF_AT(std::uint8_t, referenceMatrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, referenceMatrixBytes, 0));
    if (CSF_AT(std::uint8_t, inverseMatrixBytes, 4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseMatrixBytes, 0));

    const std::uint8_t ownsRootMatrix = rootMatrix.pad_0004[0];
    if (ownsRootMatrix)
        return static_cast<std::uint8_t>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(rootMatrix.field_0000)));
    return ownsRootMatrix;
}

HOOK(0x006E9080, GfxHAnim_AlignAnimationYaw, "auto");
