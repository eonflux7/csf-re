#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(uint32_t*, __cdecl, RwMatrixRotate, 0x006248B0, (uint32_t* a1, float* a2, float a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixTranslate, 0x00624B30, (int a1, float* a2, int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwIm3DTransform, 0x0063CED0, (int a1, unsigned int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwIm3DEnd, 0x0063CFA0, ());
CSF_FUNC(int, __cdecl, RwIm3DRenderIndexedPrimitive, 0x0063CFD0, (int a1, int a2, int a3));

CSF_GLOBAL(float, g_GfxIm3DAngleScale, 0x00861A48);

static int __stdcall GfxIm3D_DrawCircleWithDiameters(
    int centerObject, int orientationObject, unsigned char* color)
{
    alignas(8) std::uint8_t vertexBytes[0x480];
    alignas(8) std::uint8_t indexBytes[0x8C];
    alignas(8) std::uint8_t pointBytes[0xC];

    auto* center = reinterpret_cast<void*>(centerObject);
    auto* orientation = reinterpret_cast<void*>(orientationObject);

    auto* centerMatrix = RwFrameGetLTM(CSF_AT(std::uint32_t, center, 0x4));
    auto* orientationMatrix =
        RwFrameGetLTM(CSF_AT(std::uint32_t, orientation, 0x4));
    auto* rotationMatrix = RwMatrixCreate();

    const float radius = CSF_AT(float, center, 0x14);
    CSF_AT(float*, indexBytes, 0x88) =
        reinterpret_cast<float*>(
            reinterpret_cast<std::uint8_t*>(centerMatrix) + 0x30);

    int angleUnits = 0;
    auto* vertex = vertexBytes;
    do
    {
        CSF_AT(float, pointBytes, 0x0) =
            radius * CSF_AT(float, orientationMatrix, 0x0);
        CSF_AT(float, pointBytes, 0x4) =
            radius * CSF_AT(float, orientationMatrix, 0x4);
        CSF_AT(float, pointBytes, 0x8) =
            radius * CSF_AT(float, orientationMatrix, 0x8);

        const float angle = static_cast<float>(angleUnits) * g_GfxIm3DAngleScale;
        RwMatrixRotate(
            rotationMatrix,
            reinterpret_cast<float*>(
                reinterpret_cast<std::uint8_t*>(orientationMatrix) + 0x20),
            angle, 0);
        RwMatrixTranslate(
            reinterpret_cast<int>(rotationMatrix),
            CSF_AT(float*, indexBytes, 0x88), 2);
        RwV3dTransformPoints(
            reinterpret_cast<int>(pointBytes),
            reinterpret_cast<int>(pointBytes), 1,
            reinterpret_cast<int>(rotationMatrix));

        const std::uint32_t red = color[0];
        const std::uint32_t green = color[1];
        const std::uint32_t blue = color[2];
        const std::uint32_t packedColor =
            (((red | 0xFFFFFF00) << 8) | green) << 8 | blue;

        CSF_AT(float, vertex, 0x8) = CSF_AT(float, pointBytes, 0x8);
        CSF_AT(std::uint32_t, vertex, 0x18) = packedColor;
        CSF_AT(float, vertex, 0x0) = CSF_AT(float, pointBytes, 0x0);
        CSF_AT(float, vertex, 0x4) = CSF_AT(float, pointBytes, 0x4);

        vertex += 0x24;
        angleUnits += 360;
    } while (angleUnits < 11520);

    auto* indices = reinterpret_cast<std::uint16_t*>(indexBytes);
    int indexCount = 0;
    for (int i = 0; i < 32; ++i)
    {
        indices[indexCount++] = static_cast<std::uint16_t>(i);
        indices[indexCount++] = static_cast<std::uint16_t>(i + 1);
    }
    indices[indexCount - 1] = 0;
    indices[indexCount++] = 0;
    indices[indexCount++] = 16;
    indices[indexCount] = 8;
    indices[indexCount + 1] = 24;

    if (RwIm3DTransform(reinterpret_cast<int>(vertexBytes), 32, 0, 2))
    {
        RwIm3DRenderIndexedPrimitive(1, reinterpret_cast<int>(indexBytes), 68);
        RwIm3DEnd();
    }

    return RwMatrixDestroy(rotationMatrix);
}

HOOK(0x006E3250, GfxIm3D_DrawCircleWithDiameters, "auto");
