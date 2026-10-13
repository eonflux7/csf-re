#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwIm3DTransform, 0x0063CED0, (int a1, unsigned int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwIm3DEnd, 0x0063CFA0, ());
CSF_FUNC(int, __cdecl, RwIm3DRenderIndexedPrimitive, 0x0063CFD0, (int a1, int a2, int a3));
CSF_FUNC(int, __stdcall, GfxIm3d_RenderCircularCap, 0x006E33E0, (float a1, float a2, uint8_t* a3, float* a4));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);
CSF_GLOBAL(float, g_LabelBackgroundOffsetScale, 0x00834388);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);
CSF_GLOBAL(float, g_CylinderAngularStep, 0x00861A50);
CSF_GLOBAL(float, g_CylinderFarOffset, 0x00861A54);
CSF_GLOBAL(double, g_CylinderCapAngle, 0x00861A58);

static int __fastcall S_006e3740_RenderColoredCylinder(
    S_006e3740* /*self*/, void* /*edx*/, int shape, std::uint8_t* color)
{
    auto* shapeData = reinterpret_cast<std::uint8_t*>(shape);
    float* matrix = reinterpret_cast<float*>(
        RwFrameGetLTM(CSF_AT(std::uint32_t, shapeData, 0x4)));
    const float scale = CSF_AT(float, shapeData, 0x14);

    const float rightX = scale * matrix[0];
    const float rightY = scale * matrix[1];
    const float rightZ = scale * matrix[2];
    const float upX = scale * matrix[4];
    const float upY = scale * matrix[5];
    const float upZ = scale * matrix[6];
    const float forwardX = scale * matrix[8];
    const float forwardY = scale * matrix[9];
    const float forwardZ = scale * matrix[10];

    const float nearOffset =
        -(std::cos(g_CylinderCapAngle) * g_ScoreboardRefreshInterval);
    const float nearX = nearOffset * forwardX;
    const float nearY = nearOffset * forwardY;
    const float nearZ = nearOffset * forwardZ;
    const float farX = forwardX * g_CylinderFarOffset;
    const float farY = forwardY * g_CylinderFarOffset;
    const float farZ = forwardZ * g_CylinderFarOffset;

    // Forty 36-byte immediate-mode vertices. Unspecified components are zero.
    alignas(8) std::uint8_t vertexBytes[0x5A0] = {};
    alignas(8) std::uint8_t indexBytes[0x224];
    auto* indices = reinterpret_cast<std::uint16_t*>(indexBytes);

    for (unsigned int vertex = 0; vertex < 0x28; vertex += 2)
    {
        const double angle =
            static_cast<double>(vertex) * g_CylinderAngularStep;
        const double cosine = std::cos(angle);
        const double sineRadius = std::sin(angle) * g_ControlHoldThreshold;
        const float radialUpX = sineRadius * upX;
        const float radialUpY = sineRadius * upY;
        const float radialUpZ = sineRadius * upZ;
        const double cosineRadius = cosine * g_ControlHoldThreshold;
        const float radialRightX = cosineRadius * rightX;
        const float radialRightY = cosineRadius * rightY;
        const float radialRightZ = cosineRadius * rightZ;

        auto* nearVertex = vertexBytes + vertex * 36;
        const float nearPositionX =
            nearX + radialRightX + radialUpX + matrix[12];
        const float nearPositionY =
            nearY + radialRightY + radialUpY + matrix[13];
        const float nearPositionZ =
            nearZ + matrix[14] + radialRightZ + radialUpZ;
        CSF_AT(float, nearVertex, 0x0) = nearPositionX;
        CSF_AT(float, nearVertex, 0x8) = nearPositionZ;
        CSF_AT(float, nearVertex, 0x4) = nearPositionY;

        auto* farVertex = vertexBytes + (vertex + 1) * 36;
        const float farPositionX =
            farX + radialRightX + radialUpX + matrix[12];
        const float farPositionY =
            farY + radialRightY + radialUpY + matrix[13];
        const float farPositionZ =
            farZ + matrix[14] + radialRightZ + radialUpZ;
        CSF_AT(float, farVertex, 0x0) = farPositionX;
        CSF_AT(float, farVertex, 0x8) = farPositionZ;
        CSF_AT(float, farVertex, 0x4) = farPositionY;
    }

    const std::uint32_t packedColor =
        color[2] |
        ((static_cast<std::uint32_t>(color[1]) |
          ((static_cast<std::uint32_t>(color[0]) | 0xFFFF8000u) << 8)) << 8);

    for (unsigned int group = 0; group < 8; ++group)
    {
        auto* vertices = vertexBytes + group * 180;
        CSF_AT(std::uint32_t, vertices, 0x18) = packedColor;
        CSF_AT(std::uint32_t, vertices + 36, 0x18) = packedColor;
        CSF_AT(std::uint32_t, vertices + 72, 0x18) = packedColor;
        CSF_AT(std::uint32_t, vertices + 108, 0x18) = packedColor;
        CSF_AT(std::uint32_t, vertices + 144, 0x18) = packedColor;
    }

    unsigned int indexCount = 0;
    unsigned int vertex = 0;
    unsigned int nextFarVertex = 1;
    do
    {
        indices[indexCount++] = static_cast<std::uint16_t>(vertex);
        indices[indexCount++] = static_cast<std::uint16_t>(vertex + 1);
        vertex += 2;
        nextFarVertex += 2;
        const auto nextNear =
            static_cast<std::uint16_t>(vertex % 0x28);
        indices[indexCount++] = nextNear;
        indices[indexCount++] = nextNear;
        indices[indexCount++] = static_cast<std::uint16_t>(vertex - 1);
        indices[indexCount++] =
            static_cast<std::uint16_t>(nextFarVertex % 0x28u);
    }
    while (nextFarVertex < 0x29);

    for (unsigned int triangle = 0; triangle < 0x12; ++triangle)
    {
        indices[indexCount++] = 0;
        indices[indexCount++] = static_cast<std::uint16_t>(2 * triangle + 2);
        indices[indexCount++] = static_cast<std::uint16_t>(2 * triangle + 4);
    }

    for (unsigned int triangle = 0; triangle < 0x12; ++triangle)
    {
        indices[indexCount++] = 1;
        indices[indexCount++] = static_cast<std::uint16_t>(2 * triangle + 5);
        indices[indexCount++] = static_cast<std::uint16_t>(2 * triangle + 3);
    }

    if (RwIm3DTransform(reinterpret_cast<int>(vertexBytes), 40, 0, 0))
    {
        RwIm3DRenderIndexedPrimitive(3, reinterpret_cast<int>(indices), 228);
        RwIm3DEnd();
    }

    const float capOffset = scale * g_LabelBackgroundOffsetScale;
    return GfxIm3d_RenderCircularCap(
        static_cast<float>(g_CylinderCapAngle), capOffset, color, matrix);
}

HOOK(0x006E3740, S_006e3740_RenderColoredCylinder, "auto");
