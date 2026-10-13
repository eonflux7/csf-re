#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_GLOBAL(float, g_CircleAngleStep, 0x00860858);

static int __fastcall S_006c3820_AppendCircleOutline(
    S_006c3820* self, void* /*edx*/, float* center, float radius,
    std::uint8_t* color, int transform)
{
    const std::uint32_t firstVertex = self->field_000c;

    alignas(8) std::uint8_t transformedBytes[0xC];
    alignas(8) std::uint8_t radialBytes[0x14];
    float* transformed = reinterpret_cast<float*>(transformedBytes);
    float* radial = reinterpret_cast<float*>(radialBytes);

    for (int i = 0; i < 32; ++i)
    {
        const float angle = static_cast<float>(i) * g_CircleAngleStep;
        radial[2] = 0.0f;
        transformed[0] = 0.0f;
        transformed[1] = 0.0f;
        transformed[2] = 0.0f;
        radial[0] = std::cos(angle) * radius;
        radial[1] = std::sin(angle) * radius;

        RwV3dTransformVectors(
            reinterpret_cast<int>(transformedBytes),
            reinterpret_cast<int>(radialBytes), 1, transform);

        const float x = transformed[0] + center[0];
        const std::uint32_t vertexIndex = self->field_000c;
        const float y = transformed[1] + center[1];
        const std::uint32_t vertexBuffer = self->field_0004;
        const float z = transformed[2] + center[2];

        CSF_AT(float, reinterpret_cast<void*>(vertexBuffer), 36 * vertexIndex) = x;
        CSF_AT(float, reinterpret_cast<void*>(self->field_0004),
               36 * self->field_000c + 4) = y;
        CSF_AT(float, reinterpret_cast<void*>(self->field_0004),
               36 * self->field_000c + 8) = z;

        const std::uint32_t green = color[1];
        const std::uint32_t blue = color[2];
        const std::uint32_t alpha = color[3];
        const std::uint32_t red = color[0];
        const std::uint32_t packedColor =
            (alpha << 24) | (red << 16) | (green << 8) | blue;

        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0004),
               36 * self->field_000c + 24) = packedColor;
        ++self->field_000c;
        self->field_002c = 0;
    }

    std::uint32_t indexEnd = 0;
    for (int i = 1; i < 32; ++i)
    {
        const std::uint32_t vertex = firstVertex + static_cast<std::uint32_t>(i);
        CSF_AT(std::uint16_t, reinterpret_cast<void*>(self->field_0010),
               2 * self->field_0018) = static_cast<std::uint16_t>(vertex - 1);

        const std::uint32_t nextIndex = self->field_0018 + 1;
        void* indices = reinterpret_cast<void*>(self->field_0010);
        self->field_0018 = nextIndex;
        CSF_AT(std::uint16_t, indices, 2 * nextIndex) =
            static_cast<std::uint16_t>(vertex);

        indexEnd = self->field_0018 + 1;
        self->field_0018 = indexEnd;
    }

    CSF_AT(std::uint16_t, reinterpret_cast<void*>(self->field_0010),
           2 * indexEnd) = static_cast<std::uint16_t>(firstVertex);

    const std::uint32_t closingIndex = self->field_0018 + 1;
    self->field_0018 = closingIndex;
    CSF_AT(std::uint16_t, reinterpret_cast<void*>(self->field_0010),
           2 * closingIndex) = static_cast<std::uint16_t>(firstVertex + 31);

    const std::uint32_t result = self->field_0018 + 1;
    self->field_0018 = result;
    self->field_001c = 1;
    return static_cast<int>(result);
}

HOOK(0x006C3820, S_006c3820_AppendCircleOutline, "auto");
