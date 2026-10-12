#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static float* __stdcall GfxClumpLoader_ComputeVertexAverage(
    float* result, int /*unused*/, std::uint32_t* vertexData)
{
    float sumX = g_fZero;
    float sumY = 0.0f;
    float sumZ = 0.0f;

    float divisor = static_cast<float>(
        CSF_AT(std::int32_t, vertexData, 0x8));
    const std::int32_t count = CSF_AT(std::int32_t, vertexData, 0x8);
    if (count < 0)
        divisor += g_UnsignedIntegerBias;

    const float weight = g_TransitionOne / divisor;
    std::uint32_t processed = 0;

    if (count >= 4)
    {
        auto* vertices = CSF_AT(std::uint8_t*, vertexData, 0x0);
        std::uint32_t groups =
            (static_cast<std::uint32_t>(count - 4) >> 2) + 1;
        processed = 4 * groups;

        do
        {
            for (std::uint32_t index = 0; index < 4; ++index)
            {
                auto* vertex = vertices + 12 * index;
                sumX += weight * CSF_AT(float, vertex, 0x0);
                sumY += weight * CSF_AT(float, vertex, 0x4);
                sumZ += weight * CSF_AT(float, vertex, 0x8);
            }
            vertices += 48;
        } while (--groups);
    }

    if (processed < static_cast<std::uint32_t>(count))
    {
        auto* vertex =
            CSF_AT(std::uint8_t*, vertexData, 0x0) + 12 * processed;
        std::uint32_t remaining =
            static_cast<std::uint32_t>(count) - processed;

        do
        {
            sumX += weight * CSF_AT(float, vertex, 0x0);
            sumY += weight * CSF_AT(float, vertex, 0x4);
            sumZ += weight * CSF_AT(float, vertex, 0x8);
            vertex += 12;
        } while (--remaining);
    }

    result[0] = sumX;
    result[1] = sumY;
    result[2] = sumZ;
    return result;
}

HOOK(0x006FE5F0, GfxClumpLoader_ComputeVertexAverage, "auto");
