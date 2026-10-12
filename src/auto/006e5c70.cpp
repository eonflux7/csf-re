#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using std::uint32_t;

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwFrameUpdateObjects, 0x006236A0, (int a1));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwFrameTransform, 0x00623860, (int a1, int a2, int a3));
CSF_FUNC(float*, __cdecl, RwMatrixInvert, 0x00624950, (float* a1, float* a2));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixTransform, 0x00624C70, (uint32_t* a1, uint32_t* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, GfxObjectInstances_DivideVector, 0x006AB870, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(void, __fastcall, GfxMatrix_ForwardVec3AtOffset30, 0x006AE460, (S_006add20* self, void* , int a2, int a3, float* a4));
CSF_FUNC(uint32_t*, __cdecl, GfxFrames_GetWorldBounds, 0x006C0570, (uint32_t* output, int objectAddress, float attachmentFlag));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ObjectFadeInnerRadiusRatio, 0x00861C7C);

static double __fastcall GfxObjectInstances_RepositionToViewAndGetFade(
    void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t boundsBytes[0x18];
    alignas(8) std::uint8_t extentBytes[0x0C];
    alignas(8) std::uint8_t halfExtentBytes[0x18];
    alignas(8) std::uint8_t transformedBytes[0x0C];
    alignas(8) std::uint8_t displacementBytes[0x0C];
    alignas(8) std::uint8_t inverseBytes[0x08];
    alignas(8) std::uint8_t matrixBytes[0x1C];

    auto* bounds = reinterpret_cast<float*>(boundsBytes);
    auto* extent = reinterpret_cast<float*>(extentBytes);
    auto* halfExtent = reinterpret_cast<float*>(halfExtentBytes);
    auto* transformed = reinterpret_cast<float*>(transformedBytes);
    auto* displacement = reinterpret_cast<float*>(displacementBytes);
    auto& matrix = *reinterpret_cast<S_006adb40*>(matrixBytes);

    float fade = 1.0f;

    const int globalObject = GetGlobalValue_6C5760();
    void* viewProvider = CSF_AT(void*, globalObject, 0xF0);
    void* view = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, viewProvider, 0)[0x10 / 4])(viewProvider, nullptr);
    float* viewPosition = reinterpret_cast<float* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, view, 0)[0x10 / 4])(view, nullptr);

    uint32_t frame = CSF_AT(uint32_t, CSF_AT(void*, self, 0x0C), 0x04);
    std::memcpy(&matrix.field_0008, &frame, sizeof(frame));

    uint32_t* worldMatrix = RwMatrixCreate();
    std::memcpy(worldMatrix, RwFrameGetLTM(frame), 0x40);

    matrix.field_0000 = reinterpret_cast<uint32_t>(worldMatrix);
    matrix.pad_0004[0] = 0;

    // This flag occupies a float argument slot, but the caller passes raw 1.
    const uint32_t attachmentBits = 1;
    float attachmentFlag;
    std::memcpy(&attachmentFlag, &attachmentBits, sizeof(attachmentFlag));
    GfxFrames_GetWorldBounds(
        reinterpret_cast<uint32_t*>(boundsBytes),
        CSF_AT(int, self, 0x0C),
        attachmentFlag);

    transformed[0] = bounds[0];
    halfExtent[3] = bounds[3];
    transformed[1] = bounds[1];
    halfExtent[4] = bounds[4];
    extent[0] = transformed[0] - halfExtent[3];
    transformed[2] = bounds[2];
    halfExtent[5] = bounds[5];
    extent[1] = transformed[1] - halfExtent[4];
    extent[2] = transformed[2] - halfExtent[5];

    GfxObjectInstances_DivideVector(extent, nullptr, halfExtent, 2.0f);

    float* radiusComponent = halfExtent;
    if (!(halfExtent[1] < halfExtent[0]))
        radiusComponent = halfExtent + 1;

    const float outerRadius = *radiusComponent;
    const float innerRadius = outerRadius * g_ObjectFadeInnerRadiusRatio;

    auto* storedMatrix = reinterpret_cast<void*>(matrix.field_0000);
    displacement[2] = CSF_AT(float, storedMatrix, 0x38) - viewPosition[2];
    displacement[1] = CSF_AT(float, storedMatrix, 0x34) - viewPosition[1];
    displacement[0] = CSF_AT(float, storedMatrix, 0x30) - viewPosition[0];

    CSF_AT(float*, inverseBytes, 0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(std::uint8_t, inverseBytes, 0x04) = 1;

    // The packet also identifies this call address as a vtable reference.
    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        &matrix, nullptr, &CSF_AT(float*, inverseBytes, 0));

    transformed[0] = 0.0f;
    transformed[1] = 0.0f;
    transformed[2] = 0.0f;
    RwV3dTransformVectors(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(displacement),
        1,
        reinterpret_cast<int>(CSF_AT(float*, inverseBytes, 0)));

    displacement[2] = transformed[2];
    displacement[0] = transformed[0];
    displacement[1] = transformed[1];

    if (displacement[2] < g_fZero)
    {
        const float distance = std::sqrt(
            displacement[0] * displacement[0] +
            displacement[1] * displacement[1]);

        if (distance <= innerRadius)
            fade = 0.0f;
        else if (distance < outerRadius)
            fade = (distance - innerRadius) / (outerRadius - innerRadius);
    }

    GfxMatrix_ForwardVec3AtOffset30(
        reinterpret_cast<S_006add20*>(&matrix),
        nullptr, 3, 1, viewPosition);

    std::memcpy(&frame, &matrix.field_0008, sizeof(frame));
    const uint32_t parentFrame = CSF_AT(uint32_t, frame, 0x04);
    if (parentFrame)
    {
        uint32_t* parentInverse = RwMatrixCreate();
        uint32_t* parentWorld = RwFrameGetLTM(parentFrame);
        RwMatrixInvert(
            reinterpret_cast<float*>(parentInverse),
            reinterpret_cast<float*>(parentWorld));
        RwMatrixTransform(worldMatrix, parentInverse, 2);

        std::memcpy(&frame, &matrix.field_0008, sizeof(frame));
        RwFrameTransform(
            static_cast<int>(frame), reinterpret_cast<int>(worldMatrix), 0);
        RwFrameUpdateObjects(static_cast<int>(frame));
        RwMatrixDestroy(parentInverse);
    }

    RwMatrixDestroy(worldMatrix);
    if (CSF_AT(std::uint8_t, inverseBytes, 0x04))
        RwMatrixDestroy(
            reinterpret_cast<uint32_t*>(CSF_AT(float*, inverseBytes, 0)));
    if (matrix.pad_0004[0])
        RwMatrixDestroy(reinterpret_cast<uint32_t*>(matrix.field_0000));

    return fade;
}

HOOK(0x006E5C70, GfxObjectInstances_RepositionToViewAndGetFade, "auto");
